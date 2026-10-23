/*
 * A115588  Number of distinct prime numbers necessary to represent a natural
 *          number n > 1.
 * https://oeis.org/A115588
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A115588:  Number of distinct prime numbers necessary
   to represent n (>1) upon recursive factorization of powers.
   The function returns 0 for n < 2. */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Helper: factor x into primes (pr) and exponents (ex).
   The caller must provide arrays of size at least 32.
   *cnt receives the number of distinct prime factors. */
static void factor_uint64(uint64_t x, uint64_t pr[], uint64_t ex[],
                          size_t *cnt)
{
    size_t i = 0;
    if (x == 0) { *cnt = 0; return; }

    if (x % 2ULL == 0ULL) {
        uint64_t e = 0;
        while (x % 2ULL == 0ULL) { x /= 2ULL; ++e; }
        pr[i] = 2ULL; ex[i] = e; ++i;
    }
    if (x % 3ULL == 0ULL) {
        uint64_t e = 0;
        while (x % 3ULL == 0ULL) { x /= 3ULL; ++e; }
        pr[i] = 3ULL; ex[i] = e; ++i;
    }
    for (uint64_t p = 5ULL; p <= x / p; p += 6ULL) {
        if (x % p == 0ULL) {
            uint64_t e = 0;
            while (x % p == 0ULL) { x /= p; ++e; }
            pr[i] = p; ex[i] = e; ++i;
        }
        if (x % (p+2ULL) == 0ULL) {
            uint64_t e = 0;
            while (x % (p+2ULL) == 0ULL) { x /= (p+2ULL); ++e; }
            pr[i] = p+2ULL; ex[i] = e; ++i;
        }
    }
    if (x > 1ULL) {
        pr[i] = x; ex[i] = 1ULL; ++i;
    }
    *cnt = i;
}

/* Recursive worker that collects distinct primes used in factorization
   tree of value 'v'.  The array 'seen' holds primes found so far;
   'seen_cnt' is the current number of entries.  The function never
   increases the array beyond 128 elements, which is far more than
   necessary for any input up to 2^64. */
static void collect_primes(uint64_t v,
                           uint64_t seen[], int *seen_cnt)
{
    if (v <= 1ULL) return;
    uint64_t pr[32], ex[32];
    size_t cnt;
    factor_uint64(v, pr, ex, &cnt);
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = pr[i];
        /* add p if not already present */
        bool present = false;
        for (int j = 0; j < *seen_cnt; ++j) {
            if (seen[j] == p) { present = true; break; }
        }
        if (!present) {
            if (*seen_cnt < 128) { seen[*seen_cnt] = p; ++(*seen_cnt); }
        }
        /* recurse on exponent if >1 */
        if (ex[i] > 1ULL) collect_primes(ex[i], seen, seen_cnt);
    }
}

seq_t a(idx_t n)
{
    if (n < 2ULL) return 0;   /* undefined for n < 2 */
    /* buffer to store primes seen during this call */
    uint64_t seen[128];
    int seen_cnt = 0;
    collect_primes((uint64_t)n, seen, &seen_cnt);
    return (seq_t)seen_cnt;
}
