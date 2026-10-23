/*
 * A039452  Numbers whose base-11 representation has the same nonzero number
 *          of 1's and 6's.
 * https://oeis.org/A039452
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A039452:
    Numbers whose base‑11 representation contains the same non‑zero
    number of 1's and 6's.
    This implementation searches sequentially for the nth such number
    and memoizes the results.  No external libraries are required.  */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static state for memoization */
    static seq_t *seq  = NULL;   /* array of found terms   */
    static size_t    len  = 0;    /* number of terms stored*/
    static size_t    cap  = 0;    /* allocation capacity   */
    static seq_t     cur  = 1;    /* next integer to test  */

    if (n == 0) return 0;        /* undefined for n <= 0 */

    /* grow array until we have at least n terms */
    while (len < (size_t)n) {
        /* test current value */
        seq_t temp   = cur;
        int   cnt1   = 0;
        int   cnt6   = 0;
        while (temp > 0) {
            unsigned digit = (unsigned)(temp % 11);
            if (digit == 1)   ++cnt1;
            else if (digit == 6) ++cnt6;
            temp /= 11;
        }
        if (cnt1 > 0 && cnt1 == cnt6) {
            if (len == cap) {
                size_t newcap = (cap == 0) ? 4 : cap * 2;
                seq = (seq_t *)realloc(seq, newcap * sizeof(seq_t));
                cap = newcap;
            }
            seq[len++] = cur;
        }
        ++cur;
    }
    return seq[(size_t)n - 1];
}
