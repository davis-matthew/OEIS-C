/*
 * A152035  Expansion of g.f. (1-2*x^2)/(1-2*x-2*x^2).
 * https://oeis.org/A152035
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*(a(n-1)+a(n-2)) for n >= 3; a(0)=1, a(1)=2, a(2)=4
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 44, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t, if needed */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)2;
    if (n == 2)
        return (seq_t)4;

    /* a_nm2 = a(n-2), a_nm1 = a(n-1) */
    seq_t a_nm2 = (seq_t)2;   /* a(1)   */
    seq_t a_nm1 = (seq_t)4;   /* a(2)   */
    seq_t a_n   = (seq_t)0;   /* placeholder for the current a(n) */

    for (idx_t i = 3; i <= n; ++i) {
        a_n = (a_nm1 + a_nm2) * (seq_t)2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_n;
}
