/*
 * A175872  Write n in binary. Consider the 0's and 1 as a list. (*) If the
 *          new list consists entirely of 1's, then a(n) = the number of 1's,
 *          and end. Otherwise, construct a new list made up of the lengths,
 *          written in order, of the runs of similarly-valued terms from the
 *          previous list. Go to *.
 * https://oeis.org/A175872
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, nice
 * Formula:   not stated
 * Range:     u8 n <= 9828, u16 n <= 9828, u32 n <= 9828, u64 n <= 9828,
 *            u128 n <= 9828, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Iteratively replace binary digits of n by run lengths until all 1's;
   return the length of the final list. */
#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* n should be >=1 */

    /* Determine number of bits of n */
    size_t blen = 0;
    idx_t tmp = n;
    while (tmp) {
        ++blen;
        tmp >>= 1;
    }

    /* arrays large enough to hold the largest possible list (<= blen) */
    seq_t cur[65];
    seq_t nxt[65];

    /* Build initial list of bits MSB -> LSB */
    for (size_t i = 0; i < blen; ++i) {
        size_t shift = blen - 1 - i;
        cur[i] = (seq_t)((n >> shift) & 1ULL);
    }
    size_t len = blen;

    /* Repeat until every element is 1 */
    while (1) {
        int all_ones = 1;
        for (size_t i = 0; i < len; ++i) {
            if (cur[i] != (seq_t)1) {
                all_ones = 0;
                break;
            }
        }
        if (all_ones)
            return (seq_t)len;

        /* Compute run lengths of cur */
        size_t next_len = 0;
        for (size_t i = 0; i < len; ) {
            seq_t val = cur[i];
            size_t run = 1;
            while (i + run < len && cur[i + run] == val)
                ++run;
            nxt[next_len++] = (seq_t)run;
            i += run;
        }

        /* Copy back for next iteration */
        for (size_t i = 0; i < next_len; ++i)
            cur[i] = nxt[i];
        len = next_len;
    }
}
