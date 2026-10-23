/*
 * A192153  Monotonic ordering of nonnegative differences 3^i-7^j, for 40>=
 *          i>=0, j>=0.
 * https://oeis.org/A192153
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th element of the sorted, unique * nonnegative
 *            differences 3^i – 7^j for 0 ≤ i ≤ 40, j ≥ 0
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 43, u64 n <= 43, u128 n <= 43,
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
    /* Lazy static cache of the sequence values. */
    static seq_t *vals = NULL;
    static size_t count = 0;
    static size_t capacity = 0;

    if (vals == NULL) {
        /* Rough upper bound for the number of differences. */
        capacity = 2000;
        vals = (seq_t *)malloc(sizeof(seq_t) * capacity);
        if (vals == NULL)
            return (seq_t)0;   /* allocation failure → 0 */

        size_t sz = 0;
        for (idx_t i = 0; i <= 40; ++i) {

            /* 3^i as a 128‑bit integer to avoid overflow. */
            __uint128_t pow3 = 1;
            for (idx_t k = 0; k < i; ++k)
                pow3 *= 3;

            /* Iterate over j with 7^j ≤ 3^i. */
            __uint128_t pow7 = 1;
            while (pow7 <= pow3) {
                __uint128_t diff = pow3 - pow7;
                if (sz == capacity) {
                    capacity *= 2;
                    seq_t *tmp = (seq_t *)realloc(vals, sizeof(seq_t) * capacity);
                    if (tmp == NULL)
                        return (seq_t)0;   /* allocation failure → 0 */
                    vals = tmp;
                }
                vals[sz++] = (seq_t)diff;   /* cast to seq_t – works for all builds */

                /* Prepare next 7^j, but stop before overflow. */
                if (pow7 > pow3 / 7)
                    break;
                pow7 *= 7;
            }
        }

        /* Sort and remove duplicates. */
        qsort(vals, sz, sizeof(seq_t), cmp_seq);
        size_t uniq = 0;
        for (size_t i = 0; i < sz; ++i) {
            if (uniq == 0 || cmp_seq(&vals[uniq - 1], &vals[i]) != 0) {
                vals[uniq++] = vals[i];
            }
        }
        count = uniq;
    }

    if (n == 0 || n > (idx_t)count)
        return (seq_t)0;
    return vals[(size_t)n - 1];
}
