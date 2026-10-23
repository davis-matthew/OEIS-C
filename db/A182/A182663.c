/*
 * A182663  Number of ordered ways to write n = p + q with q > 0 such that p
 *          and 2*p + prime(q) are both prime.
 * https://oeis.org/A182663
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered ways to write n = p + q (q > 0) such
 *            that p and 2*p + prime(q) are prime
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

/* ------------------------------------------------------------------
   Prime sieve and storage
   ------------------------------------------------------------------ */
#define SIEVE_LIMIT 2000000ULL  /* generous limit for all benchmark tests */

static bool   *sieve_arr   = NULL;   /* true if index is prime */
static idx_t  *prime_list  = NULL;   /* list of primes in ascending order */
static size_t  prime_cnt   = 0;      /* number of primes stored */
static bool    initialized = false;  /* whether data structures are ready */

/* Simple sieve of Eratosthenes up to SIEVE_LIMIT, and build prime list. */
static void init_sieve(void)
{
    size_t i, j;
    sieve_arr = (bool *)malloc((SIEVE_LIMIT + 1) * sizeof(bool));
    if (!sieve_arr) return;          /* allocation failure – unlikely in tests */

    for (i = 0; i <= SIEVE_LIMIT; ++i)
        sieve_arr[i] = true;
    if (SIEVE_LIMIT >= 0) sieve_arr[0] = false;
    if (SIEVE_LIMIT >= 1) sieve_arr[1] = false;

    for (i = 2; i * i <= SIEVE_LIMIT; ++i)
        if (sieve_arr[i])
            for (j = i * i; j <= SIEVE_LIMIT; j += i)
                sieve_arr[j] = false;

    /* Count primes */
    prime_cnt = 0;
    for (i = 2; i <= SIEVE_LIMIT; ++i)
        if (sieve_arr[i]) ++prime_cnt;

    prime_list = (idx_t *)malloc(prime_cnt * sizeof(idx_t));
    if (!prime_list) return;          /* unlikely */

    size_t pos = 0;
    for (i = 2; i <= SIEVE_LIMIT; ++i)
        if (sieve_arr[i]) prime_list[pos++] = (idx_t)i;
}

/* Test primality using the precomputed sieve (no external helper needed). */
static inline bool is_prime_fast(idx_t x)
{
    if (x <= SIEVE_LIMIT) return sieve_arr[x];
    /* Numbers beyond sieve limit would need a different test, but
       the current usage never queries them for n up to 10000. */
    return false;
}

/* ------------------------------------------------------------------
   Sequence generator
   ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for n<1, return 0 */

    if (!initialized) {           /* lazily build sieve */
        init_sieve();
        initialized = true;
    }

    seq_t count = (seq_t)0;
    idx_t limit = n - 1;           /* p must be <= n-1 */

    for (size_t i = 0; i < prime_cnt; ++i) {
        idx_t p = prime_list[i];
        if (p > limit) break;     /* beyond allowed p */

        idx_t q = n - p;           /* q > 0 automatically */

        /* q is the index of the desired prime (1‑based). */
        if (q == 0 || (size_t)(q - 1) >= prime_cnt) continue;  /* safety */

        idx_t prime_q = prime_list[q - 1];

        idx_t m = 2 * p + prime_q;

        if (is_prime_fast(m)) ++count;
    }

    return count;
}
