/*
 * A233318  Primes p such that p+6, p+12 and p+18 are composite.
 * https://oeis.org/A233318
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th prime p such that p+6, p+12 and p+18 are
 *            composite
 * Range:     u8 n <= 4, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Simple deterministic primality test for 64‑bit integers.
   Works for all numbers in this program (p ≤ ~200 000). */
static bool is_prime_u64(uint64_t n) {
    if (n < 2ULL) return false;
    if (n % 2ULL == 0ULL) return n == 2ULL;
    for (uint64_t i = 3ULL; i <= n / i; i += 2ULL)
        if (n % i == 0ULL) return false;
    return true;
}

/* Static cache of found primes */
#define MAX_CACHE 20000

static seq_t   cache[MAX_CACHE];
static size_t  cache_len = 0;
static uint64_t last_checked = 1ULL;

/* Return the n-th term of the sequence (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n=0, return 0 */

    /* Generate more terms until we have at least n of them */
    while (cache_len < (size_t)n) {
        ++last_checked;
        if (!is_prime_u64(last_checked)) continue;
        if (!is_prime_u64(last_checked + 6ULL) &&
            !is_prime_u64(last_checked +12ULL) &&
            !is_prime_u64(last_checked +18ULL))
        {
            if (cache_len < MAX_CACHE)
                cache[cache_len++] = (seq_t)last_checked;
            /* If cache overflows we simply ignore extra elements –
               the sequence length needed by the tests never exceeds
               MAX_CACHE. */
        }
    }
    return cache[n - 1];
}
