/*
 * A175713  Expansion of 1/(1 - x - 4*x^2 + 4*x^3 - 2*x^4).
 * https://oeis.org/A175713
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n‑1) + 4*a(n‑2) – 4*a(n‑3) + 2*a(n‑4) a(0)=1, a(1)=1,
 *            a(2)=5, a(3)=5
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 28, u64 n <= 57, u128 n <= 113,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *buf      = NULL;
    static idx_t  buf_size = 0;
    static idx_t  last     = 0;
    const seq_t init[4] = { (seq_t)1, (seq_t)1, (seq_t)5, (seq_t)5 };

    if (n < 4)
        return init[(size_t)n];

    /* Allocate buffer on first use */
    if (buf == NULL) {
        buf_size = n + 1;
        buf = (seq_t *)malloc(sizeof(seq_t) * buf_size);
        if (buf == NULL)
            return init[0];
        buf[0] = init[0];
        buf[1] = init[1];
        buf[2] = init[2];
        buf[3] = init[3];
        last = 3;
    }
    /* Grow buffer if required */
    else if (n + 1 > buf_size) {
        idx_t new_size = buf_size * 2;
        if (new_size < n + 1)
            new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(buf, sizeof(seq_t) * new_size);
        if (tmp == NULL)
            return init[0];
        buf     = tmp;
        buf_size = new_size;
    }

    /* Compute terms up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t val = buf[i - 1]
                    + ((seq_t)4) * buf[i - 2]
                    - ((seq_t)4) * buf[i - 3]
                    + ((seq_t)2) * buf[i - 4];
        buf[i] = val;
    }
    last = n;
    return buf[n];
}
