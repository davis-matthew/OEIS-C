/*
 * A119330  Expansion of (1-x)^2/((1-x)^4-2x^4).
 * https://oeis.org/A119330
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4*a(n-1) - 6*a(n-2) + 4*a(n-3) + a(n-4) (n ≥ 4), with
 *            initial values a(0)=1, a(1)=2, a(2)=3, a(3)=4
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 29, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *buf      = NULL;   /* cache of computed terms   */
    static idx_t  Bsize    = 0;      /* current allocated size    */
    static const seq_t init[4] = {1, 2, 3, 4};

    /* Small indices are returned directly. */
    if (n < 4) 
        return init[n];

    /* Grow or initialise the cache to at least n+1 elements. */
    idx_t need = n + 1;
    if (buf == NULL) {
        /* Allocate once with the required size. */
        buf   = (seq_t *)malloc(need * sizeof(seq_t));
        if (buf == NULL) return 0;          /* out of memory – abort gracefully */
        /* Initialise the first four values. */
        for (idx_t i = 0; i < 4; ++i)
            buf[i] = init[i];
        /* Compute all required terms up to index n. */
        for (idx_t i = 4; i < need; ++i) {
            buf[i] = 4 * buf[i-1] - 6 * buf[i-2] + 4 * buf[i-3] + buf[i-4];
        }
        Bsize = need;
        return buf[n];
    }

    /* If the cache is too small, grow it and compute new entries. */
    if (need > Bsize) {
        seq_t *tmp = (seq_t *)realloc(buf, need * sizeof(seq_t));
        if (tmp == NULL) return 0;          /* out of memory – abort gracefully */
        buf = tmp;
        for (idx_t i = Bsize; i < need; ++i) {
            buf[i] = 4 * buf[i-1] - 6 * buf[i-2] + 4 * buf[i-3] + buf[i-4];
        }
        Bsize = need;
    }

    /* Cache hit – simply return the stored value. */
    return buf[n];
}
