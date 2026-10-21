/*
 * A053798  Number of basis partitions of n+16 with Durfee square size 4.
 * https://oeis.org/A053798
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 83, u32 n <= 3381, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A053798 – number of basis partitions of n+16 with Durfee square size 4
 * Generating function: (1+q)(1+q^2)(1+q^3)(1+q^4)/((1-q)(1-q^2)(1-q^3)(1-q^4))
 * The coefficient of q^n in this series gives a(n).
 * The algorithm expands the generating function by successive convolution
 * with the series 1+2q^k+2q^{2k}+… for k=1,2,3,4.
 * It runs in O(n) time per recomputation, which is O(n) overall for the
 * largest n requested.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* cached values a(0)…a(max_computed-1) */
static idx_t cache_len = 0;      /* number of stored terms */

/* compute all terms up to and including `upto` (0‑based) */
static void compute_up_to(idx_t upto)
{
    if (cache_len > upto) return;               /* already available */

    /* allocate temporary arrays of the required size */
    seq_t *tmp1 = (seq_t *)malloc((upto + 1) * sizeof(seq_t));
    seq_t *tmp2 = (seq_t *)malloc((upto + 1) * sizeof(seq_t));
    if (!tmp1 || !tmp2) abort();

    /* initialise tmp1: coefficient of q^0 = 1, others 0 */
    for (idx_t i = 0; i <= upto; ++i)
        tmp1[i] = (i == 0) ? (seq_t)1 : (seq_t)0;

    /* successively multiply by the series for k = 1..4 */
    for (idx_t k = 1; k <= 4; ++k) {
        /* tmp2 will hold the result of convolution with (1+2q^k+2q^{2k}+…) */
        for (idx_t r = 0; r < k; ++r) {
            seq_t cum = (seq_t)0;
            for (idx_t pos = r; pos <= upto; pos += k) {
                tmp2[pos] = tmp1[pos] + (seq_t)2 * cum;
                cum += tmp1[pos];
            }
        }
        /* swap tmp1 and tmp2 for the next iteration */
        seq_t *old = tmp1;
        tmp1 = tmp2;
        tmp2 = old;
    }

    /* free old cache, if any, and replace with the new results */
    if (cache) free(cache);
    cache = tmp1;                    /* tmp1 now holds the final array */
    cache_len = upto + 1;

    /* tmp2 is no longer needed */
    free(tmp2);
}

/* the required external function */
seq_t a(idx_t n)
{
    compute_up_to(n);
    return cache[n];
}
