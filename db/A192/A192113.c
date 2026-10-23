/*
 * A192113  Monotonic ordering of nonnegative differences 4^i-2^j, for
 *          40>=i>=0, j>=0.
 * https://oeis.org/A192113
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   compare_seq_t, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A192113: monotonic ordering of non‑negative differences 4^i – 2^j
   for 0 ≤ i ≤ 40 and j ≥ 0.  The values are enumerated, sorted and
   deduplicated once, then indexed with 1‑based numbering. */

#include <stdlib.h>
#include <string.h>
#include "../utils/pow2.h"        /* provides seq_t pow2(idx_t exp) */
#include "../utils/compare_seq_t.h" /* provides int compare_seq_t(const void*,const void*) */

static seq_t *sorted_vals = NULL;   /* distinct sorted values */
static size_t sorted_len = 0;       /* number of stored values */

/* Build the list lazily on first call. */
static void build(void)
{
    if (sorted_vals != NULL) return;   /* already built */

    /* Count of all (i,j) pairs: Σ(2i+1) for i=0..40 = 41*41 = 1681 */
    const size_t total = 41 * 41;
    seq_t *tmp = (seq_t *)malloc(total * sizeof(seq_t));
    if (!tmp) return;  /* memory failure: let caller handle zero return later */

    size_t k = 0;
    for (idx_t i = 0; i <= 40; ++i) {
        seq_t pow4i = pow2(2 * i);              /* 4^i = 2^(2i) */
        for (idx_t j = 0; j <= 2 * i; ++j) {    /* ensure 4^i – 2^j ≥ 0 */
            tmp[k++] = pow4i - pow2(j);         /* diff fits in seq_t */
        }
    }

    /* Sort all differences */
    qsort(tmp, total, sizeof(seq_t), compare_seq_t);

    /* Remove duplicates */
    size_t uniq = 0;
    for (size_t i = 0; i < total; ++i) {
        if (i == 0 || compare_seq_t(&tmp[i], &tmp[i - 1]) != 0) {
            tmp[uniq++] = tmp[i];
        }
    }

    /* Store the unique sorted list in a static array */
    sorted_vals = (seq_t *)malloc(uniq * sizeof(seq_t));
    if (sorted_vals) {
        memcpy(sorted_vals, tmp, uniq * sizeof(seq_t));
        sorted_len = uniq;
    }

    free(tmp);
}

seq_t a(idx_t n)
{
    if (sorted_vals == NULL) build();   /* ensure list exists */

    /* Sequence is 1‑based; return 0 for out‑of‑range indices. */
    if (n < 1 || n > sorted_len) return 0;
    return sorted_vals[n - 1];
}
