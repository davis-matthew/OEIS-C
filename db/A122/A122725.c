/*
 * A122725  a(n) = A000670(n)^2.
 * https://oeis.org/A122725
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = A000670(n)^2 (A000670 = ordered Bell numbers, the number
 *            of preferential arrangements of n elements)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static cache for the ordered Bell numbers B_n = A000670(n) */
static seq_t *bell_cache = NULL;   /* array of B_0 ... B_{max_n} */
static idx_t bell_len   = 0;        /* number of entries currently stored */

/* ensure the cache contains values up to index n */
static void ensure_bell(idx_t n)
{
    if (bell_cache == NULL) {
        /* initialise with B_0 = 1 */
        bell_cache = (seq_t *)malloc(sizeof(seq_t));
        if (bell_cache == NULL) return;   /* allocation failure → no result */
        bell_cache[0] = (seq_t)1;
        bell_len = 1;
    }
    if (n + 1 <= bell_len) return;        /* already computed */

    /* grow the array to hold indices up to n */
    seq_t *new_cache = (seq_t *)realloc(bell_cache,
                                        (n + 1) * sizeof(seq_t));
    if (new_cache == NULL) return;         /* allocation failure → no result */
    bell_cache = new_cache;

    /* compute missing ordered Bell numbers */
    for (idx_t i = bell_len; i <= n; ++i) {
        seq_t sum    = (seq_t)0;
        seq_t binom  = (seq_t)1;          /* C(i,0) = 1 */
        idx_t lim    = i;                 /* we sum over k = 0..i-1 */

        /* recurrence: B_i = Σ_{k=0}^{i-1} C(i,k) * B_k */
        for (idx_t k = 0; k < lim; ++k) {
            sum += bell_cache[k] * binom;
            if (k + 1 < lim)
                binom = binom * (seq_t)(i - k) / (seq_t)(k + 1);
        }
        bell_cache[i] = sum;
    }
    bell_len = n + 1;
}

/* OEIS sequence A122725: a(n) = A000670(n)^2 */
seq_t a(idx_t n)
{
    ensure_bell(n);
    seq_t v = bell_cache[n];
    return v * v;
}
