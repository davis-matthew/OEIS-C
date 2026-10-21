/*
 * A228839  Erroneous version of A000453.
 * https://oeis.org/A228839
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   not stated
 * Range:     i8 n <= 6, i16 n <= 7, i32 n <= 7, i64 n <= 7, i128 n <= 7,
 *            big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula (correct Stirling numbers of the second kind S(n,4) with the
   known typo for n = 5):
   a(n) = (4^n - 4*3^n + 6*2^n - 4) / 24   (for all n ≠ 5)
   a(5) is incorrectly listed as 20 in this sequence. */
#include <stddef.h>
#include "../utils/seq_realloc.h"

static seq_t *S = NULL;          /* 2‑D table flattened: (n*(kmax+1)+k) */
static idx_t  max_n = 0;         /* largest n for which rows are filled */
static size_t cap    = 0;        /* capacity of the flattened array */

/* Return the erroneous A228839 value for given n (n ≥ 1). */
seq_t a(idx_t n)
{
    const idx_t kmax = 4;
    const size_t row_size = kmax + 1;          /* 5 columns: k = 0..4 */

    /* Ensure storage can accommodate rows up to n. */
    size_t needed = (size_t)(n + 1) * row_size;
    if (needed > cap) {
        S = seq_realloc(S, cap, needed);
        cap = needed;
    }

    /* Initialize row 0 when starting. */
    if (max_n == 0 && S == NULL) {
        /* This will never happen because seq_realloc with needed >=5*
           sets S non‑NULL, but keep for safety. */
        S = seq_realloc(NULL, 0, needed);
        cap = needed;
    }
    if (max_n == 0) {
        seq_t *row0 = &S[0];
        row0[0] = 1;                     /* S(0,0) */
        for (idx_t k = 1; k <= kmax; ++k)
            row0[k] = 0;
    }

    /* Populate rows up to n if necessary. */
    if (n > max_n) {
        for (idx_t curr = max_n + 1; curr <= n; ++curr) {
            seq_t *curr_row = &S[(size_t)curr * row_size];
            seq_t *prev_row = &S[(size_t)(curr - 1) * row_size];
            curr_row[0] = 0;                 /* S(n,0) = 0 for n>0 */
            for (idx_t k = 1; k <= kmax; ++k) {
                curr_row[k] = prev_row[k - 1] + (seq_t)k * prev_row[k];
            }
        }
        max_n = n;
    }

    /* Retrieve the value for k = 4. */
    seq_t result = S[(size_t)n * row_size + 4];

    /* Apply the known typo for n = 5. */
    if (n == 5)
        result = (seq_t)20;

    return result;
}
