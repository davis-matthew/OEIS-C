/*
 * A192154  Monotonic ordering of nonnegative differences 7^i-3^j, for 40>=
 *          i>=0, j>=0.
 * https://oeis.org/A192154
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 14, u32 n <= 41, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A192154: non‑negative differences 7^i – 3^j (0 ≤ i ≤ 40, j ≥ 0),
   sorted increasingly and without duplicates. */

#include <stdlib.h>
#include <stdbool.h>

/* The types are supplied by the build system. */
extern seq_t a(idx_t n);

/* ---- internal helper: compare two seq_t values for qsort ---- */
static int seq_compare(const void *p1, const void *p2)
{
    const seq_t *a = (const seq_t *)p1;
    const seq_t *b = (const seq_t *)p2;
    if (*a < *b) return -1;
    if (*a > *b) return 1;
    return 0;
}

/* ---- global cache for the sequence ---- */
static seq_t *sequence = NULL;   /* pointer to the sorted distinct values */
static idx_t  seq_len  = 0;       /* number of values stored */

/* ---- build the entire sequence on first request ---- */
static void build_sequence(void)
{
    if (sequence != NULL) return;   /* already built */

    /* Pre‑compute powers of 7 up to 7^40. */
    const size_t MAX_I = 41;            /* indices 0 … 40 inclusive */
    seq_t *pow7 = (seq_t *)malloc(MAX_I * sizeof(seq_t));
    pow7[0] = 1;
    for (size_t i = 1; i < MAX_I; ++i)
        pow7[i] = pow7[i - 1] * (seq_t)7;

    /* Pre‑compute powers of 3 until the first one exceeds 7^40. */
    seq_t target = pow7[40];
    size_t j_max = 0;
    seq_t *pow3 = (seq_t *)malloc(sizeof(seq_t));   /* pow3[0] = 1 */
    pow3[0] = 1;
    for (size_t j = 1; ; ++j) {
        seq_t val = pow3[j - 1] * (seq_t)3;
        size_t new_size = j + 1;
        seq_t *tmp = (seq_t *)realloc(pow3, new_size * sizeof(seq_t));
        if (tmp == NULL) { free(pow7); free(pow3); exit(1); }
        pow3 = tmp;
        pow3[j] = val;
        if (val > target) { j_max = j - 1; break; }
    }

    /* Count total number of differences to allocate array. */
    size_t total_terms = 0;
    for (size_t i = 0; i < MAX_I; ++i) {
        for (size_t j = 0; j <= j_max; ++j) {
            if (pow3[j] > pow7[i]) break;
            ++total_terms;
        }
    }

    /* Collect all differences. */
    seq_t *all = (seq_t *)malloc(total_terms * sizeof(seq_t));
    size_t pos = 0;
    for (size_t i = 0; i < MAX_I; ++i) {
        for (size_t j = 0; j <= j_max; ++j) {
            if (pow3[j] > pow7[i]) break;
            all[pos++] = pow7[i] - pow3[j];
        }
    }

    /* Sort the array. */
    qsort(all, pos, sizeof(seq_t), seq_compare);

    /* Keep only distinct values. */
    seq_t *uniq = (seq_t *)malloc(pos * sizeof(seq_t));
    idx_t uniq_cnt = 0;
    if (pos > 0) {
        uniq[uniq_cnt++] = all[0];
        for (size_t k = 1; k < pos; ++k) {
            if (seq_compare(&all[k], &uniq[uniq_cnt - 1]) != 0)
                uniq[uniq_cnt++] = all[k];
        }
    }

    /* Finalise the cache. */
    sequence = uniq;
    seq_len  = uniq_cnt;

    /* Free temporaries. */
    free(all);
    free(pow3);
    free(pow7);
}

/* ---- public function required by the exercise ---- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined but harmless */

    build_sequence();              /* lazily build if not done */

    if (n > seq_len) return 0;      /* out of bounds */

    return sequence[n - 1];
}
