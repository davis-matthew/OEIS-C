/*
 * A122550  Floor of the slanted side of a right trapezoid formed by 3
 *          consecutive primes.
 * https://oeis.org/A122550
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor( sqrt( p_{n+1}^2 + (p_n - p_{n+2})^2 ) ), where p_k
 *            is the k-th prime. This computes the length of the slanted side
 *            of the right trapezoid formed by three consecutive primes
 * Range:     u8 n <= 2, u16 n <= 29, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Validated: partial, pass, 58 terms through n = 58 (2026-11-01)
 * Helpers:   build_primes_upto, ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include "../utils/build_primes_upto.h"
#include "../utils/ipow_i32.h"

static size_t _prime_limit_for_idx(idx_t n) {
    /* Rough upper bound for the n-th prime using n*(log n + log log n) + 3 */
    if (n < 6) return 15; /* first 6 primes < 15 */
    double nn = (double)n;
    double bound = nn * (log(nn) + log(log(nn))) + 3.0;
    return (size_t)bound;
}

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    /* n is 1-based */
    static uint64_t *primes = NULL;
    static idx_t prime_cnt = 0;
    static idx_t max_index = 0;        /* largest index of primes fetched */

    if (n < 1) return 0;    /* undefined; return 0 */

    idx_t need = n + 2;     /* we need primes up to index n+2 (1‑based) */

    if (need > max_index) {
        /* Rebuild prime table up to required index */
        idx_t limit = _prime_limit_for_idx(need);
        free(primes);                  /* free previous allocation (if any) */
        build_primes_upto(limit, &primes, &prime_cnt);
        /* prime_cnt should now be >= need */
        max_index = need;
    }

    /* Convert the required primes to seq_t */
    seq_t p1 = (seq_t)primes[n - 1];
    seq_t p2 = (seq_t)primes[n];
    seq_t p3 = (seq_t)primes[n + 1];

    /* Compute absolute difference between p1 and p3 */
    seq_t d = (p3 > p1) ? (seq_t)(p3 - p1) : (seq_t)(p1 - p3);

    seq_t sum = (p2 * p2) + (d * d);
    return floor_sqrt(sum);
}
