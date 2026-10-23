/*
 * A192172  Monotonic ordering of nonnegative differences 10^i-4^j, for
 *          40>=i>=0, j>=0.
 * https://oeis.org/A192172
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 19, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: Enumerate all non‑negative differences 10^i–4^j
   for 0 ≤ i ≤ 40, 0 ≤ j with 10^i ≥ 4^j, store the distinct values,
   sort them increasingly, and return the n‑th value (1‑based). */

#include <stdlib.h>

static int cmp_seq(const void *a, const void *b)
{
    seq_t va = *(const seq_t *)a;
    seq_t vb = *(const seq_t *)b;
    if (va < vb) return -1;
    if (va > vb) return 1;
    return 0;
}

seq_t a(idx_t n)
{
    /* static cache */
    static seq_t *diff = NULL;
    static size_t diff_cnt = 0;
    static size_t cap = 0;
    static int init = 0;

    if (!init) {
        /* initial capacity generous enough for all expected terms */
        cap = 1024;
        diff = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!diff) return (seq_t)0;          /* allocation failed */

        size_t used = 0;
        seq_t pow10 = 1;                    /* 10^0 */

        const idx_t MAX_I = 40;
        for (idx_t i = 0; i <= MAX_I; ++i) {
            /* inner loop over j */
            seq_t pow4 = 1;                 /* 4^0 */
            while (1) {
                if (pow4 > pow10) break;    /* should not happen, but guard */

                /* store difference, guaranteed non‑negative */
                if (used == cap) {
                    cap <<= 1;
                    seq_t *tmp = (seq_t *)realloc(diff, cap * sizeof(seq_t));
                    if (!tmp) { free(diff); return (seq_t)0; }
                    diff = tmp;
                }
                diff[used++] = pow10 - pow4;

                /* next 4^j */
                seq_t next_pow4 = pow4 * 4;
                if (next_pow4 < pow4)            /* overflow detected */
                    break;
                if (next_pow4 > pow10)           /* would exceed 10^i */
                    break;
                pow4 = next_pow4;
            }

            /* next 10^i */
            seq_t next_pow10 = pow10 * 10;
            if (next_pow10 < pow10)            /* overflow detected */
                break;
            pow10 = next_pow10;
        }

        /* sort ascending */
        qsort(diff, used, sizeof(seq_t), cmp_seq);

        /* remove duplicates */
        size_t uniq = 0;
        for (size_t i = 0; i < used; ++i) {
            if (uniq == 0 || diff[i] != diff[uniq - 1]) {
                diff[uniq++] = diff[i];
            }
        }
        diff_cnt = uniq;
        init = 1;
    }

    if (n == 0 || n > diff_cnt) return (seq_t)0;
    return diff[n - 1];
}
