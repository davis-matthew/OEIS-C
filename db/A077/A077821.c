/*
 * A077821  Expansion of 1/((1-3*x-3*x^2-3*x^3)*(1-x)).
 * https://oeis.org/A077821
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 3*a(n-4); a(0)=1, a(1)=4, a(2)=16, a(3)=64
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    switch (n) {
    case 0: return (seq_t)1;
    case 1: return (seq_t)4;
    case 2: return (seq_t)16;
    case 3: return (seq_t)64;
    default:
        seq_t a_prev4 = (seq_t)1;   /* a(n-4)   */
        seq_t a_prev3 = (seq_t)4;   /* a(n-3)   */
        seq_t a_prev2 = (seq_t)16;  /* a(n-2)   */
        seq_t a_prev1 = (seq_t)64;  /* a(n-1)   */
        for (idx_t i = 4; i <= n; ++i) {
            seq_t next = 4 * a_prev1 - 3 * a_prev4;
            a_prev4 = a_prev3;
            a_prev3 = a_prev2;
            a_prev2 = a_prev1;
            a_prev1 = next;
        }
        return a_prev1;
    }
}
