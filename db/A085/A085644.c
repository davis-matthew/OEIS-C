/*
 * A085644  a(0) = 1; a(n+1) = a(n)*2n + 2n + 1.
 * https://oeis.org/A085644
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1; a(n+1) = a(n)*2*n + 2*n + 1
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* memoised array of already computed values */
    static seq_t *buf = NULL;
    static idx_t buf_len = 0;   /* length of buf, i.e. highest index + 1 */

    if (n + 1 > buf_len) {      /* need to grow the cache */
        idx_t new_len = n + 1;
        seq_t *new_buf = (seq_t *)malloc(new_len * sizeof(seq_t));
        if (!new_buf) return (seq_t)0;  /* out of memory – unlikely in test set */

        /* copy existing values into the larger array */
        for (idx_t i = 0; i < buf_len; ++i)
            new_buf[i] = buf[i];

        free(buf);
        buf = new_buf;
        idx_t old_len = buf_len;
        buf_len = new_len;

        /* initialise a(0) if the array was just created */
        if (old_len == 0) {
            buf[0] = (seq_t)1;
            old_len = 1;
        }

        /* compute all new terms up to the requested index */
        for (idx_t i = old_len; i < new_len; ++i) {
            idx_t prev = i - 1;
            seq_t term = buf[prev] * ((seq_t)prev * 2) + ((seq_t)prev * 2 + 1);
            buf[i] = term;
        }
    }

    return buf[n];
}
