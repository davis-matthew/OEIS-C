/*
 * A084941  Octagorials: n-th polygorial for k=8.
 * https://oeis.org/A084941
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n*(3*n-2)*a(n-1), a(0) = 1
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static memoisation buffer */
    static seq_t *buf = NULL;
    static idx_t buf_used = 0;   /* number of values stored (next free index) */
    static idx_t buf_cap  = 0;   /* current allocation size */

    /* Initialise buffer with a(0) = 1 if not already done */
    if (buf_used == 0) {
        buf_cap  = 1;
        buf      = (seq_t *)malloc(sizeof(seq_t) * buf_cap);
        if (!buf) abort();
        buf[0]   = (seq_t)1;
        buf_used = 1;
    }

    /* Grow buffer up to the requested index n */
    while (buf_used <= n) {
        /* ensure we have room for index buf_used */
        if (buf_used >= buf_cap) {
            buf_cap = (buf_cap == 0) ? 1 : buf_cap << 1;   /* double */
            seq_t *tmp = (seq_t *)realloc(buf, sizeof(seq_t) * buf_cap);
            if (!tmp) {
                free(buf);
                abort();
            }
            buf = tmp;
        }

        /* compute a[buf_used] using the recurrence */
        idx_t m = buf_used;                        /* current index (>=1) */
        seq_t coeff = (seq_t)m * (seq_t)(3 * m - 2);
        buf[buf_used] = coeff * buf[buf_used - 1];
        ++buf_used;   /* next free slot */
    }

    return buf[n];
}
