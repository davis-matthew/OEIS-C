/*
 * A217843  Numbers which are the sum of one or more consecutive nonnegative
 *          cubes.
 * https://oeis.org/A217843
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 115, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A217843:  Numbers which are the sum of one or more consecutive nonnegative cubes.
   This implementation generates all such sums up to a sufficient bound,
   stores them sorted and unique, and returns the nth term (1‑based).*/
#include <stdlib.h>

static int cmp_seq_t_wrapper(const void *p, const void *q)
{
    const seq_t *x = (const seq_t *)p;
    const seq_t *y = (const seq_t *)q;
    if (*x < *y) return -1;
    if (*x > *y) return  1;
    return 0;
}

seq_t a(idx_t n)
{
    static seq_t *seq = NULL;   /* array of sorted unique sums */
    static size_t seq_len = 0;  /* number of elements in seq */

    if (seq == NULL) {          /* generate sequence once */
        /* Desired upper bound for sums – large enough for n up to ~10 000. */
        const idx_t TARGET = 20000;                 /* ensures ~ TARGET² ≈ 4·10⁸ */
        seq_t max_sum = (seq_t)(TARGET + 1) * (TARGET + 1);  /* 1‑based square */

        /* Find largest index i such that i³ ≤ max_sum. */
        idx_t cube_idx = 0;
        while ((seq_t)cube_idx * (seq_t)cube_idx * (seq_t)cube_idx <= max_sum)
            cube_idx++;

        /* Pre‑compute cubes up to cube_idx‑1. */
        seq_t *cube = (seq_t *)malloc(sizeof(seq_t) * cube_idx);
        for (idx_t i = 0; i < cube_idx; i++)
            cube[i] = (seq_t)i * i * i;

        /* Rough capacity estimate for all sums. */
        size_t cap = cube_idx * cube_idx / 2 + 100;
        seq_t *sums = (seq_t *)malloc(sizeof(seq_t) * cap);
        size_t sums_len = 0;

        /* Generate all sums of consecutive non‑negative cubes. */
        for (idx_t a = 0; a < cube_idx; a++) {
            seq_t cur = 0;
            for (idx_t b = a; b < cube_idx; b++) {
                cur += cube[b];
                if (cur > max_sum) break;
                if (sums_len == cap) {         /* grow buffer if needed */
                    cap *= 2;
                    sums = (seq_t *)realloc(sums, sizeof(seq_t) * cap);
                }
                sums[sums_len++] = cur;
            }
        }
        free(cube);

        /* Sort the sums. */
        qsort(sums, sums_len, sizeof(seq_t), cmp_seq_t_wrapper);

        /* Remove duplicates in place. */
        size_t uniq = 0;
        for (size_t i = 0; i < sums_len; i++)
            if (i == 0 || sums[i] != sums[i - 1])
                sums[uniq++] = sums[i];

        /* Shrink to actual size. */
        sums = (seq_t *)realloc(sums, sizeof(seq_t) * uniq);

        seq = sums;
        seq_len = uniq;
    }

    /* Guard against accidental out‑of‑bounds requests. */
    if (n == 0 || n > seq_len) return (seq_t)0;

    return seq[n - 1];   /* 1‑based index */
}
