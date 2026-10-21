/*
 * A085625  Numbers that are the sum of 2 squares in exactly 2 ways.
 * https://oeis.org/A085625
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 5540, u32 n <= 9972, u64 n <= 9972,
 *            u128 n <= 9972, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Brute‐force enumeration of i² + j² (i ≥ j ≥ 0).  
   Numbers that appear exactly twice are collected in ascending order.  
   The implementation increases the search bound until the required
   number of terms are found.  It is valid C11 and C++17. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* External types supplied by the build. */
extern seq_t a(idx_t n);

/* memoised sequence values */
static seq_t *terms = NULL;   /* array of found terms            */
static size_t terms_len = 0;  /* number of terms stored          */

/* bound used for the last computation */
static idx_t current_bound = 0;

/* Recompute the sequence up to at least 'need' terms. */
static void recompute(idx_t need)
{
    idx_t bound = current_bound;
    if (bound == 0) bound = 1000;            /* initial hint          */

    while (true) {
        /* Enumerate all unordered pairs (i,j) with i≥j≥0.  Count each sum. */
        uint8_t *cnt = (uint8_t*)calloc((size_t)bound + 1, 1);
        if (!cnt) abort();

        for (idx_t i = 0; (uint64_t)i * (uint64_t)i <= bound; ++i) {
            idx_t i2 = i * i;
            for (idx_t j = i; (uint64_t)j * (uint64_t)j <= bound - i2; ++j) {
                idx_t sum = i2 + j * j;
                if (sum <= bound) ++cnt[sum];
            }
        }

        /* Count how many numbers have exactly two representations. */
        size_t found = 0;
        for (idx_t x = 1; x <= bound; ++x)
            if (cnt[x] == 2) ++found;

        /* Store all such numbers in an array. */
        seq_t *new_terms = (seq_t*)malloc(found * sizeof(seq_t));
        if (!new_terms) {
            free(cnt);
            abort();
        }

        size_t k = 0;
        for (idx_t x = 1; x <= bound; ++x)
            if (cnt[x] == 2)
                new_terms[k++] = (seq_t)x;

        free(cnt);

        if (found >= need) {
            free(terms);
            terms = new_terms;
            terms_len = found;
            current_bound = bound;
            return;
        }

        /* Too few terms: try a larger bound. */
        free(new_terms);
        bound *= 2;   /* double the bound and retry */
    }
}

/* Public function: return the n-th term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;               /* undefined for n<1 */

    if (terms == NULL || (size_t)n > terms_len)
        recompute(n);

    return terms[n - 1];
}
