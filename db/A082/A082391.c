/*
 * A082391  Start with the sequence a(1) to a(4) = 1, 3, 2, 3. Then in step s,
 *          append "1", "1,2", or "1,2,3", whichever ends with a(s+2).
 * https://oeis.org/A082391
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 102, u16 n <= 102, u32 n <= 102, u64 n <= 102,
 *            u128 n <= 102, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recursive construction:  
   Start with a(1)=1, a(2)=3, a(3)=2, a(4)=3.  
   For each step s ≥ 1 append the block 1, 1,2, or 1,2,3 so that the last
   element of the block equals a(s+2).  The sequence contains only 1,2,3.  */

/* Note: seq_t and idx_t are provided by the build. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *seq = NULL;
    static idx_t seq_len = 0;
    static idx_t seq_cap = 0;
    static idx_t step  = 1;   /* next step to process (1‑based) */

    /* grow cache if needed to hold at least n terms */
    if (seq_cap < n) {
        idx_t newcap = (seq_cap > 0 ? seq_cap << 1 : 64);
        while (newcap < n) newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(seq, newcap * sizeof(seq_t));
        seq = tmp;
        seq_cap = newcap;
    }

    /* initialise first four terms on first call */
    if (seq_len == 0) {
        seq[0] = 1;
        seq[1] = 3;
        seq[2] = 2;
        seq[3] = 3;
        seq_len = 4;
        step = 1;          /* next step to process */
    }

    /* generate more terms until we have n */
    while (seq_len < n) {
        /* a[s+2] is stored at index (s+2)-1 = step+1 */
        seq_t target = seq[step + 1];

        if (target == 1) {            /* append block 1 */
            seq[seq_len++] = 1;
        } else if (target == 2) {     /* append block 1,2 */
            seq[seq_len++] = 1;
            seq[seq_len++] = 2;
        } else if (target == 3) {     /* append block 1,2,3 */
            seq[seq_len++] = 1;
            seq[seq_len++] = 2;
            seq[seq_len++] = 3;
        } else {
            /* Should never happen for this sequence. */
            seq[seq_len++] = 0;
        }
        ++step;   /* advance to next step */
    }

    return seq[n - 1];
}
