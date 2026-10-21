/*
 * A208946  Number of 4-bead necklaces labeled with numbers -n..n not allowing
 *          reversal, with sum zero with no three beads in a row equal.
 * https://oeis.org/A208946
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 36, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A208946
   Recurrence: a(n) = 3*a(n-1)-3*a(n-2)+2*a(n-3)-3*a(n-4)+3*a(n-5)-a(n-6)
   Initial terms: a(1)=5, a(2)=22, a(3)=57, a(4)=122, a(5)=223, a(6)=366
*/

#include <stddef.h>  /* for size_t if needed */

seq_t a(idx_t n)
{
    /* Handle indices below the offset. The sequence is defined for n>=1. */
    if (n == 0)
        return (seq_t)0;

    /* First six terms.  Indexing: base[0] = a(1), base[5] = a(6). */
    static const seq_t base[6] = {5, 22, 57, 122, 223, 366};

    if (n <= 6)
        return base[n - 1];

    /* Variables for the last six computed values:
     * a1 = a(n-6), a2 = a(n-5), ..., a6 = a(n-1)  */
    seq_t a1 = base[0];
    seq_t a2 = base[1];
    seq_t a3 = base[2];
    seq_t a4 = base[3];
    seq_t a5 = base[4];
    seq_t a6 = base[5];

    for (idx_t i = 7; i <= n; ++i) {
        seq_t next = 3 * a6 - 3 * a5 + 2 * a4 - 3 * a3 + 3 * a2 - a1;
        /* Shift the window forward */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = a6;
        a6 = next;
    }
    return a6;
}
