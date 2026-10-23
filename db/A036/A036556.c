/*
 * A036556  Integers which when multiplied by 3 have an odd number of 1's in
 *          their binary expansion (cf. A000069).
 * https://oeis.org/A036556
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 9303, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A036556
   a(n) = the nth positive integer m for which popcount(3*m) is odd */
#include <stdlib.h>

seq_t a(idx_t n)
{
    idx_t  found = 0;          /* number of terms found so far     */
    seq_t  cand  = 0;          /* current candidate integer       */

    while (found < n)          /* find the nth term                */
    {
        cand = cand + (seq_t)1;            /* next integer to test */

        seq_t val = cand * (seq_t)3;      /* 3*m                          */
        int parity = 0;                  /* parity of popcount → 0/1     */

        seq_t t = val;
        while (t) {                      /* count bits only via parity  */
            parity ^= (int)(t & (seq_t)1);
            t >>= 1;
        }

        if (parity)                      /* odd popcount → include      */
            ++found;
    }

    return cand;                          /* the nth term                 */
}
