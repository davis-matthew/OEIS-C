/*
 * A192168  Monotonic ordering of nonnegative differences 8^i-4^j, for 40>=
 *          i>=0, j>=0.
 * https://oeis.org/A192168
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth nonnegative difference 8^i - 4^j for 0 <= i <= 40 *
 *            sorted in ascending order
 * Range:     u8 n <= 6, u16 n <= 25, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static int cmp_seq(const void *a, const void *b)
{
    const seq_t *x = (const seq_t *)a;
    const seq_t *y = (const seq_t *)b;
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}

seq_t a(idx_t n)
{
    static seq_t *vals = NULL;
    static size_t sz = 0, cap = 0;
    static int init = 0;

    if (!init) {
        /* preallocate space for about 2000 values */
        cap  = 64;
        vals = (seq_t *)malloc(cap * sizeof(seq_t));
        sz = 0;

        seq_t pow8 = 1;
        for (idx_t i = 0; i <= 40; ++i) {
            seq_t pow4 = 1;
            idx_t jmax = (3 * i) / 2;
            for (idx_t j = 0; j <= jmax; ++j) {
                seq_t diff = pow8 - pow4;

                if (sz >= cap) {
                    cap *= 2;
                    seq_t *tmp = (seq_t *)realloc(vals, cap * sizeof(seq_t));
                    vals = tmp;
                }
                vals[sz++] = diff;

                pow4 = pow4 * 4;
            }
            pow8 = pow8 * 8;
        }

        /* sort ascending */
        qsort(vals, sz, sizeof(seq_t), cmp_seq);

        /* remove duplicates */
        size_t uniq = 0;
        for (size_t i = 0; i < sz; ++i) {
            if (uniq == 0 || vals[i] != vals[uniq - 1])
                vals[uniq++] = vals[i];
        }
        sz = uniq;

        init = 1;
    }

    if (n == 0 || n > sz)
        return 0; /* Undefined for requested index */

    return vals[n - 1];
}
