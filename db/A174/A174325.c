/*
 * A174325  Trisection A061037(3*n-2) of the Balmer spectrum numerators
 *          extended to negative indices.
 * https://oeis.org/A174325
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, frac, less
 * Formula:   a(n) = 3*a(n-4) - 3*a(n-8) + a(n-12) for n >= 12. * Initial
 *            values: a(0)=0, a(1)=-3, a(2)=3, a(3)=45, a(4)=6, a(5)=165, *
 *            a(6)=63, a(7)=357, a(8)=30, a(9)=621, a(10)=195, a(11)=957
 * Range:     i8 n <= 4, i16 n <= 62, i32 n <= 5000, i64 n <= 5000,
 *            i128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *buf = NULL;
    static size_t buf_cap = 0;
    static const seq_t init[12] = {0, -3, 3, 45, 6, 165, 63, 357, 30, 621, 195, 957};

    if (n < 12)
        return init[n];

    if (!buf || buf_cap <= (size_t)n) {
        size_t new_cap = buf_cap ? buf_cap * 2 : 12;
        while (new_cap <= (size_t)n)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(buf, new_cap * sizeof(seq_t));
        if (!tmp)
            return 0;                    /* memory exhausted – unlikely for given limits */
        buf = tmp;

        /* initialise first 12 terms on first allocation */
        if (!buf_cap) {
            for (size_t i = 0; i < 12; ++i)
                buf[i] = init[i];
        }

        /* compute new entries from the previous capacity up to new_cap-1 */
        size_t start = buf_cap ? buf_cap : 12;
        if (start < 12)
            start = 12;
        for (size_t i = start; i < new_cap; ++i) {
            buf[i] = 3 * buf[i - 4] - 3 * buf[i - 8] + buf[i - 12];
        }
        buf_cap = new_cap;
    }

    return buf[(size_t)n];
}
