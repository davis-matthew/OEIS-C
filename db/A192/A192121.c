/*
 * A192121  Monotonic ordering of nonnegative differences 8^i - 2^j, for 40 >=
 *          i >= 0, j >= 0.
 * https://oeis.org/A192121
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, fini
 * Formula:   Monotonic ordering of nonnegative differences 8^i - 2^j, 0 <= i
 *            <= 40, j >= 0, sorted ascending with duplicates removed
 * Range:     u8 n <= 10, u16 n <= 46, u32 n <= 166, u64 n <= 694,
 *            u128 n <= 2461, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static int cmp_seq(const void *p1, const void *p2)
{
    const seq_t *x = (const seq_t *)p1;
    const seq_t *y = (const seq_t *)p2;
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}

seq_t a(idx_t n)
{
    /* static cache of the dedup‑sorted sequence */
    static seq_t *uniqueVals = NULL;
    static size_t uniqCount = 0;

    if (!uniqueVals) {
        /* 2501 = Σ_{i=0}^{40} (3i+1) */
        const size_t total = 2501;
        seq_t *vals = (seq_t *)malloc(total * sizeof(seq_t));
        if (!vals) return 0; /* allocation failure */

        size_t pos = 0;
        seq_t pow8 = 1; /* 8^i */
        for (idx_t i = 0; i <= 40; ++i) {
            seq_t pow2 = 1; /* 2^j */
            for (idx_t j = 0; j <= 3 * i; ++j) {
                vals[pos++] = pow8 - pow2; /* non‑negative difference */
                pow2 *= (seq_t)2;
            }
            pow8 *= (seq_t)8;
        }

        /* sort all values */
        qsort(vals, total, sizeof(seq_t), cmp_seq);

        /* deduplicate */
        uniqueVals = (seq_t *)malloc(total * sizeof(seq_t));
        if (!uniqueVals) { free(vals); return 0; }
        uniqCount = 0;
        for (size_t i = 0; i < total; ++i) {
            if (i == 0 || vals[i] != vals[i-1]) {
                uniqueVals[uniqCount++] = vals[i];
            }
        }

        free(vals);
    }

    if (n == 0 || n > uniqCount) return 0; /* out of bounds */
    return uniqueVals[(size_t)(n - 1)];
}
