/*
 * A048006  Number of nonempty subsets of {1,2,...,n} in which exactly 1/3 of
 *          the elements are <= (n-1)/3.
 * https://oeis.org/A048006
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..floor((n-1)/3)} binom(floor((n-1)/3), k) *
 *            binom(n - floor((n-1)/3), 2*k)
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 35, u64 n <= 67, u128 n <= 132,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    idx_t L = (n - 1) / 3;         /* number of elements <= (n-1)/3   */
    idx_t H = n - L;               /* remaining elements > (n-1)/3   */

    seq_t sum = 0;                 /* accumulator for the answer      */
    seq_t binL = 1;                /* binom(L,0)                       */
    seq_t binH = 1;                /* binom(H,0)                       */
    idx_t r = 0;                   /* current r in binom(H,r) logic    */

    for (idx_t k = 1; k <= L; ++k) {
        /* update binom(L,k) from binom(L,k-1) */
        binL = (binL * (L - k + 1)) / k;

        /* update binom(H,2k) in two steps from binom(H,r) */
        r += 1;  /* r becomes 1  */
        binH = (binH * (H - r + 1)) / r;
        r += 1;  /* r becomes 2  */
        binH = (binH * (H - r + 1)) / r;
        /* now r == 2*k, binH == binom(H,2k) */

        sum += binL * binH;
    }
    return sum;
}
