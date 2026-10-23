/*
 * A170935  b(n)*b(n+1), where b() = A000930().
 * https://oeis.org/A170935
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = b(n)*b(n+1), where b(0)=b(1)=b(2)=1, b(3)=2, and for n≥4,
 *            b(n)=b(n-2)+b(n-3)+b(n-4)
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 29, u64 n <= 58, u128 n <= 116,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *b = NULL;          /* cached values of b(0), b(1), … */
    static idx_t b_len = 0;          /* number of elements stored in b */

    /* Need values up to index n+1, so at least n+2 entries are required. */
    idx_t need_len = n + 2;
    if (need_len > b_len) {
        /* Allocate or grow the array. */
        seq_t *tmp = (seq_t *)realloc(b, need_len * sizeof(seq_t));
        if (!tmp)                 /* allocation failed; return 0 */
            return (seq_t)0;
        b = tmp;

        /* Initialise new entries. */
        for (idx_t i = b_len; i < need_len; ++i) {
            if (i < 3) {
                b[i] = (seq_t)1;          /* b[0] = b[1] = b[2] = 1 */
            } else if (i == 3) {
                b[i] = (seq_t)2;          /* b[3] = 2 */
            } else {
                /* b(i) = b(i-2) + b(i-3) + b(i-4) for i ≥ 4 */
                b[i] = b[i - 2] + b[i - 3] + b[i - 4];
            }
        }
        b_len = need_len;
    }

    return b[n] * b[n + 1];
}
