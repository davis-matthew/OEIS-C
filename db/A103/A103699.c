/*
 * A103699  Add 8 to each of the preceding digits, beginning with 0.
 * https://oeis.org/A103699
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   breadth‑first traversal of a tree defined by * Node 0 has child
 *            8. * For any node with value v≠0, * if v<10 : child is v+8 *
 *            else : children are (v/10)+8 and (v%10)+8 * that is, add 8 to
 *            each decimal digit of v. * The sequence a(n) is the n‑th node in
 *            this level‑order traversal
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc */

seq_t a(idx_t n)
{
    /* static storage for the breadth‑first traversal */
    static seq_t *buf    = NULL;
    static idx_t  sz     = 0;
    static idx_t  cap    = 0;
    static idx_t  exp_idx = 0;   /* next index to expand */

    /* initialisation */
    if (buf == NULL) {
        cap = 64;
        buf = (seq_t *)malloc(cap * sizeof(seq_t));
        sz  = 1;
        buf[0] = 0;
    }

    /* extend buffer as needed */
    while (sz <= n) {
        if (exp_idx >= sz) break;          /* safety */

        seq_t val = buf[exp_idx++];

        /* generate children for this node */
        if (val == 0) {
            /* child 8 */
            if (sz == cap) {
                cap *= 2;
                buf = (seq_t *)realloc(buf, cap * sizeof(seq_t));
            }
            buf[sz++] = 8;
        } else {
            uint64_t u = (uint64_t)val;
            if (u < 10) {
                /* single digit child */
                if (sz == cap) {
                    cap *= 2;
                    buf = (seq_t *)realloc(buf, cap * sizeof(seq_t));
                }
                buf[sz++] = (seq_t)(u + 8);
            } else {
                /* two digits: high and low */
                seq_t high = (seq_t)(u / 10 + 8);
                seq_t low  = (seq_t)(u % 10 + 8);

                if (sz + 1 >= cap) {   /* need space for 2 children */
                    cap = (cap * 2) > (sz + 2) ? cap * 2 : sz + 2;
                    buf = (seq_t *)realloc(buf, cap * sizeof(seq_t));
                }
                buf[sz++] = high;
                buf[sz++] = low;
            }
        }
    }

    return buf[n];
}
