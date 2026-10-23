/*
 * A188580  Number of words of length n over an alphabet of size 5 which do
 *          not contain a run of 5 identical letters.
 * https://oeis.org/A188580
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*(a(n-1)+a(n-2)+a(n-3)+a(n-4)), with a(0)=1, a(1)=5,
 *            a(2)=25, a(3)=125, a(4)=625 for n≥5
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


seq_t a(idx_t n) {
    static seq_t *buf = NULL;
    static idx_t buf_len = 0;        /* number of entries stored (buf_len == size of buf) */

    if (n + 1 > buf_len) {           /* need to extend the table */
        idx_t needed = n + 1;
        seq_t *new_buf = (seq_t *)realloc(buf, needed * sizeof(seq_t));
        if (!new_buf) {              /* allocation failed; abort or return 0 */
            return (seq_t)0;
        }
        buf = new_buf;

        /* initialise newly added elements */
        for (idx_t i = buf_len; i <= n; ++i) {
            if (i == 0) {
                buf[i] = (seq_t)1;
            } else if (i == 1) {
                buf[i] = (seq_t)5;
            } else if (i == 2) {
                buf[i] = (seq_t)25;
            } else if (i == 3) {
                buf[i] = (seq_t)125;
            } else if (i == 4) {
                buf[i] = (seq_t)625;
            } else {
                buf[i] = (seq_t)4 * (buf[i - 1] + buf[i - 2] + buf[i - 3] + buf[i - 4]);
            }
        }
        buf_len = needed;
    }
    return buf[n];
}
