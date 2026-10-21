/*
 * A204092  The number of 1 by n Haunted Mirror Maze puzzles with a unique
 *          solution.
 * https://oeis.org/A204092
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-1) - 19*a(n-2) + 16*a(n-3) - 4*a(n-4); a(0) = 1,
 *            a(1) = 3, a(2) = 17, a(3) = 91
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *buf = NULL;
    static idx_t buf_len = 0;

    /* initialize first four values on first call */
    if (buf_len == 0) {
        buf_len = 4;
        buf = (seq_t *)malloc(buf_len * sizeof(seq_t));
        if (!buf) abort();
        buf[0] = 1;
        buf[1] = 3;
        buf[2] = 17;
        buf[3] = 91;
    }

    /* return already computed value */
    if (n < buf_len)
        return buf[n];

    /* need to extend cache up to n */
    size_t new_len = (size_t)(n + 1);
    buf = (seq_t *)realloc(buf, new_len * sizeof(seq_t));
    if (!buf)
        abort();          /* allocation failure -> terminate */
    buf_len = new_len;

    /* compute remaining terms */
    for (idx_t i = 4; i <= n; ++i) {
        seq_t t1 = buf[i-1];
        seq_t t2 = buf[i-2];
        seq_t t3 = buf[i-3];
        seq_t t4 = buf[i-4];

        /* top = 8*t1 + 16*t3   (always ≥ bottom) */
        seq_t top = 8*t1 + 16*t3;
        /* bottom = 19*t2 + 4*t4 */
        seq_t bottom = 19*t2 + 4*t4;

        buf[i] = top - bottom;
    }

    return buf[n];
}
