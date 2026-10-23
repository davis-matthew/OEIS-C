/*
 * A045442  Primes congruent to {1, 5, 7} mod 8.
 * https://oeis.org/A045442
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 38, u16 n <= 4906, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n‑th prime that is ≡ 1, 5 or 7 (mod 8).  */
/* It keeps a small static cache of primes and generates new ones on demand. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

static seq_t *acceptable = NULL;   /* primes that satisfy the residue condition */
static size_t accept_len = 0, accept_cap = 0;

static seq_t *primes = NULL;       /* all found primes (used for primality tests) */
static size_t primes_len = 0, primes_cap = 0;

static uint64_t next_candidate = 3; /* next odd number to test */
static bool initialized = false;

/* Grow a sequence buffer to at least 'need' elements */
static void grow_seq(seq_t **buf, size_t *len, size_t *cap, size_t need)
{
    if (*cap >= need) return;
    size_t new_cap = (*cap == 0) ? 16 : *cap;
    while (new_cap < need) new_cap <<= 1;
    seq_t *new_buf = (seq_t *)realloc(*buf, new_cap * sizeof(seq_t));
    *buf = new_buf;
    *cap = new_cap;
}

/* Simple trial division primality test using the primes list. */
static bool is_prime_uint64(uint64_t m)
{
    for (size_t i = 0; i < primes_len; ++i)
    {
        uint64_t p = (uint64_t)primes[i];
        if (p * p > m) break;
        if (m % p == 0) return false;
    }
    return true;
}

/* Ensure that we have at least 'n' acceptable primes in the cache. */
static void ensure_n_acceptables(idx_t n)
{
    if (!initialized)
    {
        /* Initial capacity for primes and acceptable lists */
        grow_seq(&primes, &primes_len, &primes_cap, 16);
        grow_seq(&acceptable, &accept_len, &accept_cap, 16);

        /* Prime 2 is needed for trial division */
        primes[primes_len++] = 2;
        /* 2 is not acceptable (2 mod 8) */
        initialized = true;
    }

    while (accept_len < n)
    {
        if (is_prime_uint64(next_candidate))
        {
            /* Store prime */
            grow_seq(&primes, &primes_len, &primes_cap, primes_len + 1);
            primes[primes_len++] = (seq_t)next_candidate;

            /* Check residue */
            uint8_t r = (uint8_t)(next_candidate & 7); /* next_candidate % 8 */
            if (r == 1 || r == 5 || r == 7)
            {
                grow_seq(&acceptable, &accept_len, &accept_cap, accept_len + 1);
                acceptable[accept_len++] = (seq_t)next_candidate;
            }
        }
        next_candidate += 2; /* skip even numbers */
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n < 1; return 0 as a safe guard */
    ensure_n_acceptables(n);
    return acceptable[n - 1];
}
