/*
 * A046217  First numerator and then denominator of 1/2-Pascal triangle (by
 *          row) excluding 1's and 2's.
 * https://oeis.org/A046217
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabf, nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 37, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A046217: numerators of the 1/2‑Pascal triangle (row by row), excluding fractions with numerator 1. */

#include <stdlib.h>
#include "../utils/gcd.h"

typedef struct {
    seq_t num;
    seq_t den;
} frac_t;

/* Add two fractions a/b + c/d, reduce the result. */
static inline void add_frac(seq_t a_num, seq_t a_den,
                            seq_t b_num, seq_t b_den,
                            seq_t *out_num, seq_t *out_den)
{
    seq_t num = a_num * b_den + b_num * a_den; /* may overflow only when values exceed 64‑bit */
    seq_t den = a_den * b_den;
    seq_t g = gcd(num, den);
    *out_num = num / g;
    *out_den = den / g;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* undefined for n=0, dummy return */

    /* The first two rows of the triangle produce no terms (all numerators are 1). */
    /* Start with row 2: 1/1 1/2 1/1 */
    size_t prev_len = 3;
    frac_t *prev = (frac_t *)malloc(prev_len * sizeof(frac_t));
    if (!prev) return 0;   /* allocation failure */

    prev[0].num = 1; prev[0].den = 1;
    prev[1].num = 1; prev[1].den = 2;
    prev[2].num = 1; prev[2].den = 1;

    size_t collected = 0;  /* number of terms already collected */

    for (size_t row = 3; ; ++row) {          /* generate rows 3,4,5,… */
        size_t curr_len = row + 1;
        frac_t *curr = (frac_t *)malloc(curr_len * sizeof(frac_t));
        if (!curr) { free(prev); return 0; }

        /* first and last fractions are always 1/1 */
        curr[0].num = 1; curr[0].den = 1;
        curr[curr_len-1].num = 1; curr[curr_len-1].den = 1;

        /* interior fractions by adding adjacent pairs of the previous row */
        for (size_t i = 1; i < curr_len-1; ++i) {
            add_frac(prev[i-1].num, prev[i-1].den,
                     prev[i].num,   prev[i].den,
                     &curr[i].num,  &curr[i].den);
        }

        /* collect numerators (exclude ones with numerator 1) */
        for (size_t i = 1; i < curr_len-1; ++i) {
            if (curr[i].num != (seq_t)1) {
                ++collected;
                if (collected == n) {
                    seq_t result = curr[i].num;
                    free(curr);
                    free(prev);
                    return result;
                }
            }
        }

        /* prepare for next iteration */
        free(prev);
        prev = curr;
    }
}
