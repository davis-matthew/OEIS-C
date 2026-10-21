/*
 * A092606  Fixed point of the morphism 0 -> 021, 1 -> 0, 2 -> 0; starting
 *          with a(1) = 0.
 * https://oeis.org/A092606
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) is the fixed point of the morphism 0 → 0 2 1, 1 → 0, 2 → 0,
 *            starting from 0 (offset 1)
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, free, realloc */

static unsigned char *buf = NULL;    /* current word in 0/1/2 */
static size_t buf_len = 0;           /* length of that word */

seq_t a(idx_t n)
{
    if (n == 0)  /* offset starts at 1 – return arbitrary 0 */
        return 0;

    /* Initialise the word with "0" if this is the first call. */
    if (!buf) {
        buf = (unsigned char*)malloc(sizeof(unsigned char));
        if (!buf) exit(1);
        buf[0] = 0;
        buf_len = 1;
    }

    /* Expand until we have at least n symbols. */
    while (buf_len < (size_t)n) {
        size_t old_len = buf_len;

        /* Count zeros in the old word. */
        size_t zeros = 0;
        for (size_t i = 0; i < old_len; ++i)
            if (buf[i] == 0) zeros++;

        /* New length after applying the morphism to all zeros. */
        size_t new_len = old_len + 2 * zeros;

        unsigned char *new_buf = (unsigned char*)malloc(new_len);
        if (!new_buf) exit(1);

        /* Produce the expanded word. */
        size_t p = 0;
        for (size_t i = 0; i < old_len; ++i) {
            unsigned char v = buf[i];
            if (v == 0) {            /* 0 → 0 2 1 */
                new_buf[p++] = 0;
                new_buf[p++] = 2;
                new_buf[p++] = 1;
            } else {                  /* 1 or 2 → 0 */
                new_buf[p++] = 0;
            }
        }

        free(buf);
        buf = new_buf;
        buf_len = new_len;
    }

    /* Convert the 0/1/2 value to seq_t and return. */
    return (seq_t)buf[n - 1];
}
