/*
 * A025377  Numbers that are the sum of 4 distinct nonzero squares in exactly
 *          2 ways.
 * https://oeis.org/A025377
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   the sequence is the list of integers that can be expressed * as
 *            the sum of four distinct non‑zero squares in exactly two ways. *
 *            No closed form formula is known, so a search algorithm is used
 * Range:     u8 none, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

static uint32_t *cnt = NULL;     /* count of representations per sum   */
static size_t   cnt_len = 0;     /* length of cnt array                */
static seq_t   *terms = NULL;    /* sorted list of terms after each pass */
static size_t   terms_len = 0;   /* number of terms currently stored   */
static int      max_i = 0;       /* size of the largest square used    */

/* recompute all representations up to squares of size new_max_i */
static void recompute_terms(int new_max_i)
{
    idx_t   i, j, k, l;
    size_t  max_sum = (size_t)4u * (size_t)new_max_i * (size_t)new_max_i;
    size_t  s;

    /* allocate counter array */
    free(cnt);
    cnt = (uint32_t*)calloc(max_sum + 1, sizeof(uint32_t));
    cnt_len = max_sum + 1;

    /* enumerate all 4‑tuples of distinct indices */
    for (i = 1; i <= (idx_t)new_max_i - 3; ++i) {
        seq_t sq_i = (seq_t)i * (seq_t)i;
        for (j = i + 1; j <= (idx_t)new_max_i - 2; ++j) {
            seq_t sum2 = sq_i + (seq_t)j * (seq_t)j;
            for (k = j + 1; k <= (idx_t)new_max_i - 1; ++k) {
                seq_t sum3 = sum2 + (seq_t)k * (seq_t)k;
                for (l = k + 1; l <= (idx_t)new_max_i; ++l) {
                    seq_t s_val = sum3 + (seq_t)l * (seq_t)l;
                    if (s_val <= (seq_t)max_sum)
                        ++cnt[(size_t)s_val];
                }
            }
        }
    }

    /* count how many terms we will have */
    size_t new_terms_cnt = 0;
    for (s = 1; s <= max_sum; ++s)
        if (cnt[s] == 2) ++new_terms_cnt;

    /* allocate terms array */
    free(terms);
    terms = (seq_t*)malloc(new_terms_cnt * sizeof(seq_t));
    terms_len = new_terms_cnt;

    /* fill terms array in ascending order */
    size_t idx = 0;
    for (s = 1; s <= max_sum; ++s)
        if (cnt[s] == 2)
            terms[idx++] = (seq_t)s;

    /* keep the new maximum square size */
    max_i = new_max_i;
}

/* public function required by the build */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined for n==0 */
    /* start first computation on first call */
    if (max_i == 0) recompute_terms(50);

    /* keep recomputing until we have at least n terms */
    while (terms_len < (size_t)n) {
        int new_max_i = max_i * 2;
        recompute_terms(new_max_i);
    }
    return terms[(size_t)n - 1];
}
