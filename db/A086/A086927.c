/*
 * A086927  a(n) = 10*a(n-1) + a(n-2), starting with a(0) = 2 and a(1) = 10.
 * https://oeis.org/A086927
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*a(n-1) + a(n-2), a(0) = 2, a(1) = 10
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *buf = NULL;
    static idx_t sz = 0;   /* current size of buf (number of valid entries) */

    /* If we already have the value, just return it */
    if (n < sz)
        return buf[n];

    /* Need to extend the buffer to at least n+1 entries */
    idx_t new_sz = (n + 1 > sz) ? (n + 1) : sz;

    if (new_sz > sz) {
        /* Allocate or resize the buffer */
        if (buf == NULL)
            buf = (seq_t *)malloc(new_sz * sizeof(seq_t));
        else
            buf = (seq_t *)realloc(buf, new_sz * sizeof(seq_t));

        if (buf == NULL) /* allocation failure – abort */
            exit(1);

        sz = new_sz;

        /* Ensure base cases are initialized */
        buf[0] = (seq_t)2;
        if (sz > 1)
            buf[1] = (seq_t)10;

        /* Compute new entries iteratively */
        for (idx_t i = 2; i <= n; ++i)
            buf[i] = (seq_t)10 * buf[i - 1] + buf[i - 2];
    }

    return buf[n];
}
