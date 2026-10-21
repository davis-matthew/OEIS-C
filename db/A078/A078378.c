/*
 * A078378  Total number of prime factors of numbers which can be written as
 *          sum of a positive square and a positive cube.
 * https://oeis.org/A078378
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = bigomega of the n-th number that can be written as * a
 *            positive square plus a positive cube (x^2 + y^3, x,y > 0)
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bigomega, int_cube_root, int_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/int_cube_root.h"
#include "../utils/int_sqrt.h"
#include "../utils/bigomega.h"

static seq_t *sum_values = NULL;   /* sorted unique sums (=A055394) */
static size_t sum_count = 0;       /* number of stored sums           */

/* comparison routine for sort */
static int seq_compare(const void *p1, const void *p2)
{
    const seq_t *a = (const seq_t *)p1;
    const seq_t *b = (const seq_t *)p2;
    if (*a < *b) return -1;
    if (*a > *b) return 1;
    return 0;
}

/* generate the sorted unique sums up to a given numerical limit.
 * Continues doubling the limit until the requested amount of terms is
 * available.  The generated list is stored in sum_values.
 */
static void init_sums(idx_t required)
{
    if (sum_count >= required)
        return;                     /* already enough terms */

    idx_t limit = 300000;            /* starting bound -- sufficient for n≈10⁴ */
    while (1) {
        idx_t y_max = int_cube_root(limit);
        size_t est_cap = (size_t)(y_max + 1) * (size_t)(int_sqrt(limit) + 1);
        seq_t *tmp = (seq_t *)malloc(sizeof(seq_t) * est_cap);
        if (!tmp)
            abort();                 /* allocation failure */

        size_t tot = 0;
        for (idx_t y = 1; y <= y_max; ++y) {
            idx_t y3 = y * y * y;    /* fits in 64‑bit for our limits */
            if (y3 > limit)
                break;
            idx_t rem = limit - y3;
            idx_t x_max = int_sqrt(rem);
            for (idx_t x = 1; x <= x_max; ++x) {
                tmp[tot++] = (seq_t)x * (seq_t)x + (seq_t)y3;
            }
        }

        qsort(tmp, tot, sizeof(seq_t), seq_compare);

        /* Deduplicate */
        size_t uniq = 0;
        for (size_t i = 0; i < tot; ++i) {
            if (i == 0 || tmp[i] != tmp[i - 1])
                tmp[uniq++] = tmp[i];
        }

        /* Shrink buffer to fit */
        seq_t *shrink = (seq_t *)realloc(tmp, sizeof(seq_t) * uniq);
        if (shrink) tmp = shrink; /* realloc may fail only with zero size */

        /* Replace previous list */
        if (sum_values)
            free(sum_values);
        sum_values = tmp;
        sum_count  = uniq;

        if (sum_count >= required)
            break;

        limit *= 2;                 /* insufficient – double the bound */
    }
}

/* --- public sequence function ------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)            /* offset is 1; this case is undefined */
        return 0;
    init_sums(n);          /* ensure we have at least n terms ready */

    /* the (n‑1)-th element contains the n‑th sum */
    return bigomega((idx_t)sum_values[n - 1]);
}
