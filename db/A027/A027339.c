/*
 * A027339  Number of partitions of n that do not contain 5 as a part.
 * https://oeis.org/A027339
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p(n) - p(n-5), with p(0)=1 and the pentagonal number
 *            recurrence for the partition function p(n). p(n) is the number
 *            of unrestricted integer partitions of n. The function uses a
 *            static cache to store computed values of p
 * Range:     u8 n <= 17, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* static cache of partition values p(0)..p(max)  */
static seq_t *p_cache = NULL;      /* array of p values  */
static idx_t  p_cache_len = 0;     /* number of valid entries (>=1) */
static idx_t  p_cache_cap = 0;     /* allocated capacity */

/* enlarge the cache so that it contains p(k) for all 0 <= k <= n  */
static void
ensure_partitions(idx_t n)
{
    /* initial allocation if needed */
    if (!p_cache) {
        p_cache_cap = n + 1;
        p_cache = (seq_t *)malloc(p_cache_cap * sizeof(seq_t));
        if (!p_cache) abort();
        p_cache[0] = 1;       /* p(0) = 1 */
        p_cache_len = 1;
    }
    /* grow the cache if requested size exceeds current capacity */
    if (p_cache_cap <= n) {
        p_cache_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(p_cache, p_cache_cap * sizeof(seq_t));
        if (!tmp) abort();
        p_cache = tmp;
    }
    /* compute missing partition values */
    for (idx_t i = p_cache_len; i <= n; ++i) {
        seq_t plus = 0;
        seq_t minus = 0;
        for (idx_t k = 1; ; ++k) {
            idx_t pent1 = k * (3 * k - 1) / 2;
            if (pent1 > i) break;
            seq_t term = p_cache[i - pent1];
            idx_t pent2 = k * (3 * k + 1) / 2;
            if (pent2 <= i) term = term + p_cache[i - pent2];
            if (k & 1U)               /* odd k -> plus */
                plus = plus + term;
            else                      /* even k -> minus */
                minus = minus + term;
        }
        p_cache[i] = plus - minus;
    }
    p_cache_len = n + 1;
}

/* OEIS entry a(n).  Returns the number of partitions of n that
   do not contain 5 as a part. */
seq_t
a(idx_t n)
{
    ensure_partitions(n);
    if (n < 5) {
        return p_cache[n];
    } else {
        return p_cache[n] - p_cache[n - 5];
    }
}
