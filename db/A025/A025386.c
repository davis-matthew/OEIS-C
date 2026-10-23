/*
 * A025386  Numbers that are the sum of 4 distinct nonzero squares in 2 or
 *          more ways.
 * https://oeis.org/A025386
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 55, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of A025386 (sum of 4 distinct non‑zero squares
   in at least two ways) by dynamic programming over all possible squares.
   The function is memoised using a static table that grows until it
   contains at least n terms. */

#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdint.h>

/* Global memoisation structures */
static size_t max_limit = 0;      /* current maximum sum considered */
static size_t terms_len = 0;      /* number of terms stored so far */
static seq_t *terms = NULL;       /* array of terms */

/* Rebuilds the DP tables and the terms array up to a limit that
   yields at least target_n terms. */
static void rebuild_until(size_t target_n)
{
    if (terms_len >= target_n) return;

    size_t limit = (max_limit == 0) ? 200 : max_limit * 2;

    while (1) {
        if (limit < 200) limit = 200;

        /* Dynamic programming tables: count[k][s] = how many ways
           to write sum s as a sum of k distinct squares, capped at 2. */
        uint8_t *count[5];
        for (int i = 0; i < 5; ++i) {
            count[i] = (uint8_t *)calloc(limit + 1, sizeof(uint8_t));
            if (!count[i]) exit(EXIT_FAILURE); /* abort on OOM */
        }

        count[0][0] = 1;  /* one way to form 0 with 0 squares */

        for (size_t n = 1;; ++n) {
            size_t sq = n * n;
            if (sq > limit) break;
            for (int k = 4; k >= 1; --k) {
                for (size_t s = limit; s >= sq; --s) {
                    uint8_t val = count[k][s] + count[k - 1][s - sq];
                    if (val > 2) val = 2;
                    count[k][s] = val;
                }
            }
        }

        /* Count how many sums have at least two representations */
        size_t new_len = 0;
        for (size_t s = 0; s <= limit; ++s)
            if (count[4][s] >= 2) ++new_len;

        seq_t *new_terms = (seq_t *)malloc(new_len * sizeof(seq_t));
        if (!new_terms) exit(EXIT_FAILURE);
        size_t idx = 0;
        for (size_t s = 0; s <= limit; ++s)
            if (count[4][s] >= 2) new_terms[idx++] = (seq_t)s;

        /* Replace old table and terms array */
        free(terms);
        terms = new_terms;
        terms_len = new_len;
        max_limit = limit;

        /* Free DP tables */
        free(count[4]); free(count[3]); free(count[2]); free(count[1]); free(count[0]);

        if (terms_len >= target_n) break;
        limit <<= 1;   /* try a larger limit next time */
    }
}

/* Public function requested by the OEIS entry. */
extern seq_t a(idx_t n)
{
    if (n == 0) return 0;         /* undefined for n < 1; safe fallback */

    size_t nn = (size_t)n;
    if (nn > terms_len) rebuild_until(nn);

    return terms[nn - 1];
}
