/*
 * A135566  Least prime not already in the sequence such that the n-th partial
 *          concatenation is a multiple of the n-th prime.
 * https://oeis.org/A135566
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 27, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   digit_len, next_prime, pow10, sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A135566: least prime not already in the sequence such that the
   n‑th partial concatenation is a multiple of the n‑th prime.
   Search‑based implementation. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "../utils/pow10.h"                /* seq_t pow10(idx_t e) */
#include "../utils/digit_len.h"            /* int digit_len(seq_t x) */
#include "../utils/next_prime.h"           /* uint64_t next_prime(uint64_t start) */
#include "../utils/sieve_first_n_primes.h" /* sieve_first_n_primes(idx_t n,
                                              uint32_t **primes_out,
                                              size_t *cnt_out) */

/* ---------- static state to memoize the sequence ---------- */
static seq_t *seq              = NULL;   /* computed sequence values */
static size_t seq_len          = 0;      /* number of values stored (offset 1) */
static size_t seq_cap          = 0;      /* capacity of seq array */

static bool *used              = NULL;   /* bitmap of primes already used */
static size_t used_cap         = 0;      /* capacity of used array */

static uint32_t *primes        = NULL;   /* array of first primes, 1‑based */
static size_t prime_count      = 0;      /* number of primes stored */

/* ------------------------------------------------------------------ */
/* Ensure seq has room for at least min_cap elements.                 */
/* ------------------------------------------------------------------ */
static void grow_seq(size_t min_cap)
{
    if (seq_cap < min_cap)
    {
        size_t new_cap = seq_cap ? seq_cap * 2 : 16;
        if (new_cap < min_cap) new_cap = min_cap;
        seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        seq_cap = new_cap;
    }
}

/* ------------------------------------------------------------------ */
/* Ensure the bitmap `used` can index a prime value of at least max_val. */
/* ------------------------------------------------------------------ */
static void grow_used(size_t max_val)
{
    if (used_cap <= max_val)
    {
        size_t new_cap = used_cap ? used_cap * 2 : 16;
        while (new_cap <= max_val) new_cap <<= 1;
        used = (bool *)realloc(used, new_cap * sizeof(bool));
        memset(used + used_cap, 0, (new_cap - used_cap) * sizeof(bool));
        used_cap = new_cap;
    }
}

/* ------------------------------------------------------------------ */
/* Ensure at least `need` primes are available in `primes`.             */
/* ------------------------------------------------------------------ */
static void ensure_primes(size_t need)
{
    if (prime_count < need)
    {
        uint32_t *tmp = NULL;
        size_t cnt = 0;
        sieve_first_n_primes((idx_t)need, &tmp, &cnt);
        free(primes);          /* free old array if any */
        primes = tmp;
        prime_count = cnt;
    }
}

/* ------------------------------------------------------------------ */
/* Compute a(k) (1‑based) and store it in seq[k-1].                    */
/* ------------------------------------------------------------------ */
static void compute_a_part(idx_t k)
{
    /* Ensure we have the k‑th prime for the modulus. */
    ensure_primes((size_t)k);
    seq_t pk = (seq_t)primes[k - 1];   /* the n‑th prime */

    /* Compute the concatenation of the first k‑1 terms modulo pk. */
    seq_t cur = 0;
    for (size_t i = 0; i < (size_t)(k - 1); ++i)
    {
        int len = digit_len(seq[i]);
        seq_t pow = pow10((idx_t)len) % pk;
        cur = (cur * pow + seq[i]) % pk;
    }

    /* Search for the least unused prime q such that
       (cur * 10^(len(q)) + q) mod pk == 0. */
    size_t q_idx = 0;
    for (;;)
    {
        if (q_idx >= prime_count)   /* need more primes */
        {
            uint64_t nextp = next_prime(primes[prime_count - 1] + 1);
            uint32_t newp = (uint32_t)nextp;
            uint32_t *tmp = (uint32_t *)realloc(primes,
                                                (prime_count + 1) * sizeof(uint32_t));
            primes = tmp;
            primes[prime_count++] = newp;
        }

        uint32_t q = primes[q_idx++];
        if (q >= used_cap) grow_used(q + 1);
        if (used[q]) continue;       /* already chosen elsewhere */

        int lenq = digit_len((seq_t)q);
        seq_t powq = pow10((idx_t)lenq) % pk;
        seq_t rem  = (cur * powq + q) % pk;

        if (rem == 0)
        {
            seq[k - 1] = (seq_t)q;
            used[q] = true;
            return;
        }
    }
}

/* ------------------------------------------------------------------ */
/* Public entry point as required.                                   */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Offset 1: a(1) is the first term. Behaviour for n < 1 is unspecified. */
    if (n == 0) return 0;

    if (seq_len < (size_t)n)
    {
        grow_seq((size_t)n);
        for (size_t k = seq_len + 1; k <= (size_t)n; ++k)
            compute_a_part((idx_t)k);
        seq_len = (size_t)n;
    }
    return seq[n - 1];
}
