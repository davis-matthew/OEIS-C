/*
 * A115364  a(n) = A000217(A001511(n)), where A001511 is one more than the
 *          2-adic valuation of n, and A000217(n) is the n-th triangular
 *          number, binomial(n+1, 2).
 * https://oeis.org/A115364
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, mult, nonn
 * Formula:   a(n) = binomial(v2(n)+2, 2) = (v2(n)+1)*(v2(n)+2)/2
 * Range:     u8 n <= 9657, u16 n <= 9657, u32 n <= 9657, u64 n <= 9657,
 *            u128 n <= 9657, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* v2(n) = largest exponent e such that 2^e divides n, e=0 for odd n */
    unsigned long long v2 = __builtin_ctzll((unsigned long long)n);
    /* k = v2(n)+1, then a = k*(k+1)/2 = triangular(k+1) */
    unsigned long long k = v2 + 1ULL;
    seq_t result = (seq_t)k * (k + 1ULL) / 2ULL;
    return result;
}
