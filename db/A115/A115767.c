/*
 * A115767  Integers i such that 2*i XOR 5*i = 3*i.
 * https://oeis.org/A115767
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   integers i such that 2*i XOR 5*i == 3*i
 * Range:     u8 n <= 24, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t found = 0;   /* how many solutions we have seen so far   */
    seq_t i = 0;       /* current candidate integer                 */

    for (;;) {
        /* check the defining property (note the parentheses) */
        if ((((seq_t)2 * i) ^ ((seq_t)5 * i)) == ((seq_t)3 * i)) {
            if (found == n)
                return i;     /* n‑th solution found                  */
            ++found;
        }
        ++i;                /* try the next integer                  */
    }
}
