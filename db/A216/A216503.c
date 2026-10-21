/*
 * A216503  a(n) = number of positive integers k such that n = x^2 + k*y^2 has
 *          a solution with x>0, y>0.
 * https://oeis.org/A216503
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of positive integers k such that n = x^2 + k*y^2
 *            has a solution with x>0, y>0
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* comparator for qsort on idx_t values */
static int cmp_idx_t(const void *p1, const void *p2)
{
    idx_t a = *(const idx_t*)p1;
    idx_t b = *(const idx_t*)p2;
    return (a < b) ? -1 : (a > b) ? 1 : 0;
}

/* Count distinct k with n = x^2 + k*y^2, x>0, y>0 */
seq_t a(idx_t n)
{
    if (n < 2) return 0;                 /* a(1)=0 */

    /* dynamic array of found k values */
    idx_t *ks = NULL;
    size_t ks_cap = 0;
    size_t ks_len = 0;

    /* growth helper */
    if (sizeof(idx_t) == sizeof(uint64_t)) {
        /* pre‑allocate a small initial capacity */
        ks_cap = 16;
        ks = (idx_t*)malloc(ks_cap * sizeof(idx_t));
        if (!ks) return 0;              /* allocation failure */
    }

    /* main search */
    for (idx_t x = 1; x * x < n; ++x) {
        idx_t r = n - x * x;          /* r = n - x^2 > 0 */
        for (idx_t y = 1; y * y <= r; ++y) {
            idx_t y2 = y * y;
            if (r % y2 == 0) {
                idx_t k = r / y2;
                /* append k to array */
                if (ks_len == ks_cap) {
                    size_t new_cap = ks_cap * 2;
                    idx_t *tmp = (idx_t*)realloc(ks, new_cap * sizeof(idx_t));
                    if (!tmp) {
                        free(ks);
                        return 0;          /* allocation failure */
                    }
                    ks = tmp;
                    ks_cap = new_cap;
                }
                ks[ks_len++] = k;
            }
        }
    }

    /* sort and count distinct k */
    if (ks_len > 0) {
        qsort(ks, ks_len, sizeof(idx_t), cmp_idx_t);
    }

    seq_t count = 0;
    for (size_t i = 0; i < ks_len; ++i) {
        if (i == 0 || ks[i] != ks[i - 1]) {
            count++;
        }
    }

    free(ks);
    return count;
}
