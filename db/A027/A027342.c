/*
 * A027342  Number of partitions of n that do not contain 8 as a part.
 * https://oeis.org/A027342
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p(n) - p(n-8), where p is the partition function
 * Range:     u8 n <= 16, u16 n <= 44, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Static cache for partition numbers p(0)…p(n).
   p_cache[i] holds p(i).  The cache is grown on request. */
static seq_t *p_cache = NULL;  /* pointer to first element of the array */
static idx_t p_cap   = 0;      /* allocated length of p_cache   */
static idx_t p_max   = 0;      /* largest index for which p is computed */

/* Ensure that p_cache contains partition values up to n. */
static void ensure_partition(idx_t n)
{
    /* First allocation */
    if (p_cache == NULL) {
        p_cap   = n + 1;                     /* allocate at least up to n */
        p_cache = (seq_t *)malloc(p_cap * sizeof(seq_t));
        if (!p_cache) exit(1);              /* abort on OOM */
        p_cache[0] = (seq_t)1;              /* p(0) = 1 */
        p_max = 0;
    }
    /* Grow the array if needed */
    else if (n + 1 > p_cap) {
        seq_t *tmp = (seq_t *)realloc(p_cache, (n + 1) * sizeof(seq_t));
        if (!tmp) exit(1);
        p_cache = tmp;
        p_cap   = n + 1;
    }

    /* Compute new partition values */
    for (idx_t i = p_max + 1; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        for (idx_t k = 1; ; ++k) {
            /* Generalised pentagonal numbers */
            idx_t pent1 = k * (3 * k - 1) / 2;
            idx_t pent2 = k * (3 * k + 1) / 2;
            if (pent1 > i && pent2 > i) break;

            int sign = (k & 1) ? 1 : -1; /* + for odd k, - for even k */

            if (pent1 <= i) {
                if (sign > 0) sum += p_cache[i - pent1];
                else          sum -= p_cache[i - pent1];
            }
            if (pent2 <= i) {
                if (sign > 0) sum += p_cache[i - pent2];
                else          sum -= p_cache[i - pent2];
            }
        }
        p_cache[i] = sum;
    }
    p_max = n;
}

/* External function required by the build. */
seq_t a(idx_t n)
{
    ensure_partition(n);
    seq_t result = p_cache[n];
    if (n >= 8) result -= p_cache[n - 8];
    return result;
}
