/*
 * A197122  Number of partitions of n into positive quarter-squares (cf.
 *          A002620).
 * https://oeis.org/A197122
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 87, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 1;
   a(n) = sum_{q ∈ Q, q ≤ n} a(n − q),   for n ≥ 1
   where Q = {1,2,4,6,9,12,16,20,25,…} are positive quarter-squares
   (values floor(m^2/4) for m ≥ 2). */

#include <stdlib.h>
#include <stdint.h>

static seq_t *dp_cache = NULL;      /* cache of computed partition counts */
static idx_t   dp_size  = 0;        /* number of entries in dp_cache (max n + 1) */

/* compute a(n) using dynamic programming with quarter-square parts */
seq_t a(idx_t n)
{
    if (n == 0)                 /* base case, a(0) = 1 */
        return (seq_t)1;

    /* If cache is empty or n exceeds current cached size, rebuild */
    if (dp_cache == NULL || n >= dp_size) {
        /* allocate new cache big enough for indices 0..n */
        seq_t *new_cache = (seq_t *)malloc((n + 1) * sizeof(seq_t));
        if (new_cache == NULL)
            return (seq_t)0;   /* error: cannot allocate memory */

        /* initialise all entries to 0 */
        for (idx_t i = 0; i <= n; ++i)
            new_cache[i] = (seq_t)0;

        new_cache[0] = (seq_t)1;  /* a(0) = 1 */

        /* Iterate over all quarter‑square parts q ≤ n */
        for (idx_t m = 2; ; ++m) {
            seq_t q = ((seq_t)m * (seq_t)m) / 4; /* floor(m^2/4) */
            if (q > n)
                break;
            idx_t qidx = (idx_t)q;
            for (idx_t i = qidx; i <= n; ++i) {
                new_cache[i] += new_cache[i - qidx];
            }
        }

        /* free old cache if it existed */
        if (dp_cache != NULL)
            free(dp_cache);

        dp_cache = new_cache;
        dp_size  = n + 1;
    }

    return dp_cache[n];
}
