/*
 * A133700  A051731 * A001227; a(n) = Sum_{d|n} A001227(d).
 * https://oeis.org/A133700
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Sum_{d|n} A001227(d) (number of odd divisors of d).
 *            Multiplicative form: a(2^e) = e+1, a(p^e) = (e+1)(e+2)/2 for odd
 *            prime p
 * Range:     u8 n <= 3779, u16 n <= 9931, u32 n <= 9931, u64 n <= 9931,
 *            u128 n <= 9931, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Helper: integer square root for idx_t (uint64_t). Built with
   binary search and 128‑bit multiplication to avoid overflow. */
static idx_t isqrt_idx(idx_t n)
{
    idx_t low = 0;
    idx_t high = n;
    while (low <= high) {
        idx_t mid = (low + high) >> 1;
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq == n) return mid;
        if (sq < n) {
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return high;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n=0, just guard */
    idx_t temp = n;
    seq_t result = 1;

    /* Handle powers of 2 */
    idx_t e2 = 0;
    while ((temp & 1ULL) == 0) {
        ++e2;
        temp >>= 1;
    }
    if (e2) {
        result *= (seq_t)(e2 + 1);
    }

    /* Factor remaining odd part */
    idx_t i = 3;
    idx_t r = isqrt_idx(temp);
    while (i <= r && temp > 1) {
        idx_t e = 0;
        while (temp % i == 0) {
            ++e;
            temp /= i;
        }
        if (e) {
            seq_t term = ((seq_t)(e + 1) * (seq_t)(e + 2)) / 2;
            result *= term;
            r = isqrt_idx(temp);  /* recompute sqrt after division */
        }
        i += 2;
    }

    /* Remaining prime factor with exponent 1 */
    if (temp > 1) {
        /* (1+1)(1+2)/2 = 3 */
        result *= (seq_t)3;
    }

    return result;
}
