/*
 * A024365  Areas of right triangles with coprime integer sides.
 * https://oeis.org/A024365
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n‑th smallest positive integer of the form u*v*(u² -
 *            v²) where u > v ≥ 1, gcd(u,v)=1 and exactly one of u,v is even.
 *            The sequence is sorted and duplicate values are removed
 * Range:     u8 none, u16 none, u32 n <= 22, u64 n <= 10000, u128 n <= 10000,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/gcd.h"

/* Comparator for qsort */
static int cmp_seq_t(const void *x, const void *y)
{
    const seq_t *a = (const seq_t *)x;
    const seq_t *b = (const seq_t *)y;
    if (*a < *b) return -1;
    if (*a > *b) return 1;
    return 0;
}

seq_t a(idx_t n)
{
    /* Lazily generate the whole sequence once, storing in static storage. */
    static seq_t *values = NULL;
    static size_t val_cnt = 0;

    if (val_cnt == 0) {
        /* Upper bound for u that guarantees at least 10000 distinct terms. */
        const idx_t U_LIMIT = 2000;   /* generous limit, fits in RAM quickly */

        /* Allocate a buffer large enough for all (u,v) pairs with u ≤ U_LIMIT. */
        const size_t reserve = U_LIMIT * (U_LIMIT - 1) / 2;
        values = (seq_t *)malloc(reserve * sizeof(seq_t));
        if (!values) abort();

        size_t pos = 0;
        for (idx_t u = 2; u <= U_LIMIT; ++u) {
            /* v ranges from 1 to u-1 */
            for (idx_t v = 1; v < u; ++v) {
                if (((u + v) & 1u) && gcd((seq_t)u, (seq_t)v) == 1) {
                    /* Compute area using a 64‑bit intermediate to avoid overflow
                       when seq_t is 8, 16 or 32 bits. */
                    unsigned long long uu = (unsigned long long)u * u;
                    unsigned long long vv = (unsigned long long)v * v;
                    unsigned long long diff = uu - vv;
                    unsigned long long prod = (unsigned long long)u * v * diff;
                    values[pos++] = (seq_t)prod;
                }
            }
        }

        /* Sort and remove duplicates. */
        qsort(values, pos, sizeof(seq_t), cmp_seq_t);

        size_t uniq = 0;
        for (size_t i = 0; i < pos; ++i) {
            if (uniq == 0 || values[i] != values[uniq - 1]) {
                values[uniq++] = values[i];
            }
        }
        /* Reallocate to the exact size of unique terms. */
        values = (seq_t *)realloc(values, uniq * sizeof(seq_t));
        val_cnt = uniq;
    }

    /* n is 1‑based; the sequence starts at a(1). */
    return values[(size_t)(n - 1)];
}
