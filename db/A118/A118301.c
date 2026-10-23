/*
 * A118301  Number of partitions of n into distinct parts with largest part
 *          congruent to n modulo 2.
 * https://oeis.org/A118301
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into distinct parts * whose
 *            largest part is congruent to n modulo 2
 * Range:     u8 n <= 33, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *dp_total = NULL;   /* partitions of each sum (distinct parts) */
static seq_t *dp_even  = NULL;   /* partitions whose largest part is even   */
static seq_t *dp_odd   = NULL;   /* partitions whose largest part is odd    */
static idx_t  max_n    = 0;

/* Compute a(n).  The function is memoised; it can be called in any order. */
seq_t a(idx_t n)
{
    if (n == 0) {
        /* The OEIS offset is 1; a(0) is not defined, but return 0 safely. */
        return (seq_t)0;
    }

    /* Allocate or resize the working arrays if a larger n is requested. */
    if (n > max_n) {
        size_t sz = (size_t)(n + 1);
        /* free previous allocations if any */
        free(dp_total);
        free(dp_even);
        free(dp_odd);

        dp_total = (seq_t *)calloc(sz, sizeof(seq_t));
        dp_even  = (seq_t *)calloc(sz, sizeof(seq_t));
        dp_odd   = (seq_t *)calloc(sz, sizeof(seq_t));

        if (!dp_total || !dp_even || !dp_odd) {
            /* allocation failed – return 0; this case is unlikely. */
            return (seq_t)0;
        }

        /* Initialise the empty partition: sum 0 has one representation (empty). */
        dp_total[0] = (seq_t)1;
        max_n = n;

        /* Dynamic programming for partitions into distinct parts.
         * For each part size i, update counts for all sums >= i,
         * processing sums in descending order so that each part is used at most once.
         */
        for (idx_t i = 1; i <= n; ++i) {
            idx_t par = i & 1;                      /* parity of i */
            for (idx_t s = n; s >= i; --s) {
                seq_t val = dp_total[s - i];        /* partitions of (s-i) without i */
                if (par)   dp_odd[s] += val;       /* largest part is odd   */
                else       dp_even[s] += val;      /* largest part is even  */
                dp_total[s] += val;                /* accumulate total partitions */
            }
        }
    }

    /* Result is the number of partitions of n whose largest part has the
     * same parity as n. */
    return (n & 1) ? dp_odd[n] : dp_even[n];
}
