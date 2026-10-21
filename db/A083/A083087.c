/*
 * A083087  Square table read by antidiagonals which forms a permutation of
 *          the natural numbers: T(n,0) = floor(n*x/(x-1))+1, T(n,k+1) =
 *          ceiling(x*T(n,k)), for n>=0, k>=0, where x = 1 + sqrt(2).
 * https://oeis.org/A083087
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,0) = floor(n*x/(x-1)) + 1 * T(n,k+1) = ceil(x*T(n,k)), x = 1
 *            + sqrt(2) * Sequence a is obtained by reading T(n,k) along
 *            antidiagonals. * The function uses the recurrence * T(n,1) =
 *            ceil(x*T(n,0)) ; * T(n,k+1) = 2*T(n,k) + T(n,k-1) + 1 (k >= 1) *
 *            * T(n,0) is computed as * T(n,0) = n + floor(n*...
 * Range:     u8 n <= 12, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   antidiagonal_coords, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/antidiagonal_coords.h"
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    idx_t i, j;
    /* map index to table coordinates */
    antidiagonal_coords(n, &i, &j);

    seq_t ti = (seq_t)i;

    /* T(i,0) = i + floor(i*sqrt(2)/2) + 1
     * floor(i*sqrt(2)/2) = floor( sqrt(2*i*i) ) / 2
     */
    seq_t r0 = floor_sqrt(((seq_t)2 * ti * ti));
    seq_t t0 = ti + r0 / 2 + 1;

    if (j == 0)
        return t0;

    /* T(i,1) = ceil(x*T(i,0)) = T(i,0) + floor(T(i,0)*sqrt(2)) + 1
     * floor(T(i,0)*sqrt(2)) = floor( sqrt(2*T(i,0)*T(i,0)) )
     */
    seq_t r1 = floor_sqrt(((seq_t)2 * t0 * t0));
    seq_t t1 = t0 + r1 + 1;

    if (j == 1)
        return t1;

    /* For j >= 2 use the linear recurrence:
     *     T(k+1) = 2*T(k) + T(k-1) + 1
     */
    seq_t prev2 = t0;   /* T(i,0)   */
    seq_t prev1 = t1;   /* T(i,1)   */
    seq_t curr;

    for (idx_t k = 1; k < j; ++k) {          /* j-1 iterations */
        curr = (seq_t)(((unsigned __int128)2 * prev1) + prev2 + 1);
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}
