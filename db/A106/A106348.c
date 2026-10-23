/*
 * A106348  Partial sums of a generalized Fredholm-Rueppel sequence.
 * https://oeis.org/A106348
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(2^(k+1)-2) = k+1 , * a(k) = k-1 for 2^k-2 < k < 2^(k+1)-2 *
 *            That is equivalent to: * a(n) = floor(log2(n+1)) - 1, * except
 *            when n+2 is an exact power of two, then a(n) = log2(n+2). * *
 *            The function below implements this definition for any n ≥ 0
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* The index is offset 0. */
    idx_t p = n + 2;            /* p = n + 2  (≥2) */
    idx_t q = p - 1;            /* q = p - 1  (≥1) */

    /* Compute m = floor(log2(q)).  This is the value of k in the
     * piecewise definition such that 2^k ≤ q < 2^(k+1).
     */
    idx_t m = 0;
    idx_t temp = q;
    while (temp >>= 1) m++;

    /* Compute the boundary index where the special value applies:
     * boundary = 2^(m+1) - 2
     */
    idx_t boundary;
    if (m + 1 == 64) {
        /* Avoid shift by 64 bits (undefined).  2^64 - 2 == UINT64_MAX - 1. */
        boundary = ((idx_t)(-1)) - 2;
    } else {
        boundary = (((idx_t)1) << (m + 1)) - 2;
    }

    if (n == boundary) {
        /* Special case: a(2^(m+1)-2) = m+1 */
        return (seq_t)(m + 1);
    } else {
        /* Regular case: a(k) = m-1  (m ≥ 1 for all non‑boundary indices) */
        return (seq_t)(m - 1);
    }
}
