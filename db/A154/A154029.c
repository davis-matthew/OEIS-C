/*
 * A154029  List of pairs of numbers: {n^2-1, (2*n-1)!!} such that
 *          F((2*n-1)!!) = n^2 - 1.
 * https://oeis.org/A154029
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = { (n/2)*((n/2)+2) if n is even, 1*3*5*...*n if n is odd.
 *            } (Special case: a(0) = 0.)
 * Range:     u8 n <= 8, u16 n <= 12, u32 n <= 20, u64 n <= 34, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>          /* for bool type, no other use */

seq_t a(idx_t n)
{
    /* The value for n = 0 is specified as 0. */
    if (n == 0)
        return (seq_t)0;

    /* Even indices: a(2k) = k*(k+2) */
    if ((n & 1) == 0) {
        idx_t k = n >> 1;            /* k = n/2  */
        seq_t sk = (seq_t)k;          /* convert to seq_t */
        return sk * (sk + (seq_t)2);
    }

    /* Odd indices: product of all odd numbers up to n (double factorial). */
    seq_t prod = (seq_t)1;
    for (idx_t i = 1; i <= n; i += 2) {
        prod = prod * (seq_t)i;
    }
    return prod;
}
