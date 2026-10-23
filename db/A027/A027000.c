/*
 * A027000  a(n) = Lucas(2*n+3) - (6*n+4).
 * https://oeis.org/A027000
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Lucas(2*n+3) - (6*n+4)
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 21, u64 n <= 44, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static cache for Lucas numbers: L0 = 2, L1 = 1. */
static seq_t *luc_cache = NULL;   /* array of Lucas numbers */
static idx_t  luc_len   = 0;       /* number of elements currently stored */
static idx_t  luc_cap   = 0;       /* allocated capacity */

/* Ensure luc_cache has at least 'need' elements, reallocating if necessary. */
static void ensure_luc_capacity(idx_t need)
{
    if (need <= luc_cap) return;
    idx_t new_cap = luc_cap ? luc_cap << 1 : 8;
    if (new_cap < need) new_cap = need;
    seq_t *new_ptr = (seq_t*)realloc(luc_cache, new_cap * sizeof(seq_t));
    if (!new_ptr) abort();          /* terminate on allocation failure */
    luc_cache = new_ptr;
    luc_cap = new_cap;
}

/* Compute the n-th term of sequence A027000, offset 1. */
seq_t a(idx_t n)
{
    /* index into the Lucas sequence: k = 2*n + 3 */
    idx_t k = (idx_t)2 * n + 3;

    /* Initialize cache if empty. */
    if (luc_len == 0) {
        ensure_luc_capacity(2);
        luc_cache[0] = (seq_t)2;
        luc_cache[1] = (seq_t)1;
        luc_len = 2;
    }

    /* Grow cache up to index k, computing missing Lucas numbers. */
    if (k >= luc_len) {
        ensure_luc_capacity(k + 1);
        for (idx_t i = luc_len; i <= k; ++i) {
            luc_cache[i] = luc_cache[i - 1] + luc_cache[i - 2];
        }
        luc_len = k + 1;
    }

    /* Subtract the linear term 6*n + 4. */
    seq_t sub = (seq_t)6 * (seq_t)n + (seq_t)4;

    return luc_cache[k] - sub;
}
