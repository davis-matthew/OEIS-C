/*
 * A194397  Numbers m such that Sum_{k=1..m} (<1/2 + k*r> - <k*r>) > 0, where
 *          r=sqrt(14) and < > denotes fractional part.
 * https://oeis.org/A194397
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 78, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A194397
   Numbers m such that Sum_{k=1..m} (⌊k*r+½⌋–⌊k*r⌋) > 0, where r = √14.
   The condition is equivalent to more terms having fractional part
   of k*r less than ½.
   This implementation simply scans m in increasing order,
   keeping the cumulative sum of +1/-1 according to that condition,
   and returns the n‑th value where the sum becomes positive.
*/

#include <stdlib.h>
#include <stdint.h>

/* Compute floor(sqrt(x)) for an unsigned __int128 argument. */
static uint64_t isqrt_uint128(__uint128_t x)
{
    if (x == 0) return 0;
    uint64_t lo = 0, hi = 1;
    while ((__uint128_t)hi * hi <= x) hi <<= 1;
    lo = hi >> 1;
    while (lo < hi) {
        uint64_t mid = lo + ((hi - lo + 1) >> 1);
        if ((__uint128_t)mid * mid <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    int64_t sum = 0;   /* cumulative sum of +1/-1 values   */
    size_t found = 0;  /* number of m with sum>0 found     */
    uint64_t m = 0;    /* current candidate value          */

    while (found < n) {
        ++m;
        /* compute floor(√(14·m²)) and floor(√(56·m²)) */
        __uint128_t val14 = (__uint128_t)14 * m * m;
        __uint128_t val56 = (__uint128_t)56 * m * m;
        uint64_t p = isqrt_uint128(val14);
        uint64_t q = isqrt_uint128(val56);

        /* a = +1 if fractional part < ½, else –1 */
        int a_val = (q == 2 * p) ? 1 : -1;
        sum += a_val;

        if (sum > 0) {
            ++found;
            if (found == n) return (seq_t)m;
        }
    }
    /* should never reach here */
    return 0;
}
