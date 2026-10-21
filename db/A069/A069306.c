/*
 * A069306  Number of 2 X n binary arrays with a path of adjacent 1's from
 *          upper left corner to anywhere in right hand column.
 * https://oeis.org/A069306
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1)+a(n-2) with a(2)=5, a(3)=12
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 25, u64 n <= 50, u128 n <= 100,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the (n+1)-th Pell number: P(n+1)   */
/*   P(0)=0, P(1)=1,  P(k)=2*P(k-1)+P(k-2)    */

#include <stdint.h>   /* for uint64_t, idx_t is defined elsewhere */

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 2.  We compute the Pell
       numbers up to P(n+1).  Since n >= 2, n+1 >= 3 and the
       loop will run at least twice. */
    seq_t p0 = (seq_t)0;   /* P(0) */
    seq_t p1 = (seq_t)1;   /* P(1) */
    idx_t i;
    idx_t limit = n + 1;   /* compute up to P(limit) */

    for (i = 2; i <= limit; ++i) {
        seq_t next = (seq_t)2 * p1 + p0; /* P(i) = 2*P(i-1)+P(i-2) */
        p0 = p1;
        p1 = next;
    }
    return p1;
}
