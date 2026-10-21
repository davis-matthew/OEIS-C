/*
 * A080324  Union of even squarefree numbers (A039956) and squarefree numbers
 *          for which the number of prime factors is even (A030229).
 * https://oeis.org/A080324
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) is the nth smallest integer that is either an
 * even square‑free integer or an odd square‑free integer which has an
 * even number of distinct prime factors.  The function builds the
 * sequence incrementally using a trial‑division test based on a
 * precomputed prime table. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* ----------------------------------------------------------------
 * Prime table (complimentary to the sieve) -----------------------
 * ----------------------------------------------------------------
 */

#define PRIME_SIEVE_LIMIT 1000000U /* sufficient for n up to at least 10000 */

static bool  prime_sieve_done = false;
static uint32_t prime_list[80000];   /* primes up to PRIME_SIEVE_LIMIT */
static size_t  prime_count = 0;

static void build_prime_sieve(void)
{
    if (prime_sieve_done) return;

    static bool is_prime[PRIME_SIEVE_LIMIT + 1];
    for (size_t i = 0; i <= PRIME_SIEVE_LIMIT; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;

    for (size_t i = 2; i * i <= PRIME_SIEVE_LIMIT; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= PRIME_SIEVE_LIMIT; j += i)
                is_prime[j] = false;
        }
    }

    for (size_t i = 2; i <= PRIME_SIEVE_LIMIT; ++i)
        if (is_prime[i]) prime_list[prime_count++] = (uint32_t)i;

    prime_sieve_done = true;
}

/* ----------------------------------------------------------------
 * Test whether a 32‑bit integer satisfies the sequence condition
 * ----------------------------------------------------------------
 */

static bool is_good(uint32_t x)
{
    if (x == 1) return true;  /* 1 is square‑free and ω = 0 even */

    uint32_t m = x;
    int omega = 0;
    bool squarefree = true;

    for (size_t i = 0; i < prime_count &&
           (uint64_t)prime_list[i] * prime_list[i] <= (uint64_t)m; ++i) {
        uint32_t p = prime_list[i];
        if (m % p == 0) {
            ++omega;
            m /= p;
            if (m % p == 0) { squarefree = false; break; }
        }
    }
    if (!squarefree) return false;
    if (m > 1) ++omega;  /* remaining prime factor > sqrt(original) */

    if ((x & 1) == 0) /* even number: square‑free already checked */
        return true;
    return (omega & 1) == 0;  /* odd number: need even ω */
}

/* ----------------------------------------------------------------
 * Storage for the pre‑computed sequence values --------------------
 * ----------------------------------------------------------------
 */

static seq_t *seq_terms = NULL;
static size_t seq_len  = 0;
static size_t seq_cap  = 0;

static void append_term(seq_t val)
{
    if (seq_len >= seq_cap) {
        seq_cap = (seq_cap == 0 ? 1 : seq_cap * 2);
        seq_terms = (seq_t *)realloc(seq_terms, seq_cap * sizeof(seq_t));
    }
    seq_terms[seq_len++] = val;
}

/* ----------------------------------------------------------------
 * Public function -----------------------------------------------
 * ----------------------------------------------------------------
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined, return 0 for safety */

    build_prime_sieve();           /* make primes ready */

    static uint32_t next_candidate = 1;

    /* fill the sequence until we have at least n terms */
    while (seq_len < (size_t)n) {
        uint32_t cand = next_candidate++;
        if (is_good(cand))
            append_term((seq_t)cand);
    }
    return seq_terms[n - 1];
}
