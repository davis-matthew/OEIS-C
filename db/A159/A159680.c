/*
 * A159680  The general form of the recurrences are the a(j), b(j) and n(j)
 *          solutions of the 2 equations problem: 9*n(j) + 1 = a(j)*a(j) and
 *          11*n(j) + 1 = b(j)*b(j) with positive integer numbers.
 * https://oeis.org/A159680
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=0, a(2)=40, a(n) = 399*(a(n-1)-a(n-2)) + a(n-3) for n ≥ 3
 *            (OEIS A159680)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for idx_t, seq_t */

seq_t a(idx_t n)
{
    /* Handle the first term explicitly.  For all other n we use
       the recurrence, which uses the previous three values.     */
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)40;

    /* Recurrence indices:
         nm3 = a(n-3), nm2 = a(n-2), nm1 = a(n-1)   */
    seq_t nm3 = (seq_t)0;          /* a(0) (value 0)  */
    seq_t nm2 = (seq_t)0;          /* a(1) (value 0)  */
    seq_t nm1 = (seq_t)40;         /* a(2) (value 40) */

    /* Compute a(3) … a(n) iteratively. */
    for (idx_t i = 3; i <= n; ++i) {
        seq_t cur = (seq_t)399 * (nm1 - nm2) + nm3;
        nm3 = nm2;
        nm2 = nm1;
        nm1 = cur;
    }
    return nm1;
}
