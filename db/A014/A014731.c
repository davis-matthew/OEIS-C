/*
 * A014731  Squares of even Lucas numbers.
 * https://oeis.org/A014731
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = L(3*n)^2 (L is the Lucas sequence, L(0)=2, L(1)=1,
 *            L(k)=L(k-1)+L(k-2)) Squares of even Lucas numbers
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h> /* for abort(), if needed */

seq_t a(idx_t n)
{
    /* Lucas index needed: ind = 3*n */
    idx_t ind = (idx_t)3 * n;

    /* Handle small indices directly */
    if (ind == 0) {
        return (seq_t)4;            /* L(0)=2 → 2^2 */
    }
    if (ind == 1) {
        return (seq_t)1;            /* L(1)=1 → 1^2 = 1 (rare, only for n=1/3 not integer) */
    }

    /* Iterate Lucas recurrence up to index 'ind' */
    seq_t L_prev2 = (seq_t)2;     /* L(0) */
    seq_t L_prev1 = (seq_t)1;     /* L(1) */
    seq_t L_curr  = 0;            /* placeholder */

    /* Loop from k=2 to ind */
    for (idx_t k = 2; k <= ind; ++k) {
        L_curr = L_prev1 + L_prev2;
        L_prev2 = L_prev1;
        L_prev1 = L_curr;
    }

    /* Square the Lucas number L(3*n) */
    return (seq_t)L_curr * L_curr;
}
