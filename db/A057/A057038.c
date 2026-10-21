/*
 * A057038  Let R(i,j) be the rectangle with antidiagonals 1; 2,3; 4,5,6; ...;
 *          each k is an R(i(k),j(k)) and A057038(n)=i(2n).
 * https://oeis.org/A057038
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = i(2n) where i(k) is the row index of k in the array that
 *            fills its entries anti‑diagonally: 1; 2,3; 4,5,6; 7,8,9,10;
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is computed directly from the dictionary formula.  For
   each n, let k = 2n.  Find the minimal d such that the triangular
   number T_d = d*(d+1)/2 is at least k; then the row index of k is
   r = k - T_{d-1}.  All arithmetic uses 128‑bit integers where
   necessary to avoid overflow.  The function runs in O(1) time.     */

#include <stdint.h>
#include <math.h>

seq_t a(idx_t n)
{
    /* k = 2*n might not fit into 64 bits when n is near 2^63-1 */
    __uint128_t k = (__uint128_t)n * 2ULL;

    /* Estimate d via the real quadratic formula and correct
       any rounding errors that may occur due to finite precision. */
    long double approx = sqrtl((long double)k * 8.0L + 1.0L);
    unsigned long long d = (unsigned long long)((approx - 1.0L) / 2.0L);

    /* The estimate gives floor of the required d; bump up until
       the condition T_d >= k is satisfied. */
    while ( (__uint128_t)d * (d + 1) / 2 < k )
        d++;

    /* Return the row index r = k - T_{d-1} */
    __uint128_t prev = (__uint128_t)(d - 1) * d / 2;
    unsigned long long r = (unsigned long long)(k - prev);
    return (seq_t)r;
}
