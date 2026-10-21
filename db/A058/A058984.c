/*
 * A058984  Number of partitions of n in which number of parts is not 2.
 * https://oeis.org/A058984
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p(n) - ⌊n/2⌋ where p(n) is the partition number of n
 * Range:     u8 n <= 16, u16 n <= 43, u32 n <= 127, u64 n <= 416,
 *            u128 n <= 1458, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Partition numbers are computed via Euler's pentagonal recurrence:
       p(0) = 1
       for n > 0:   p(n) = ∑_{k=1}^{∞} (-1)^{k-1}
                      [ p(n - k(3k-1)/2) + p(n - k(3k+1)/2) ]
       Only terms with non‑negative arguments are included. */
    static seq_t *p_cache = NULL;      /* cache of partition numbers */
    static idx_t cache_cap = 0;        /* allocated capacity of p_cache   */
    static idx_t next_to_compute = 0;  /* first index not yet computed */

    /* Allocate or grow the cache to hold indices up to n. */
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(p_cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;                  /* OOM – harmless for this task */
        p_cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialise base case p(0)=1 if this is the first call. */
    if (next_to_compute == 0) {
        p_cache[0] = (seq_t)1;
        next_to_compute = 1;
    }

    /* Compute missing partition numbers in order. */
    for (idx_t i = next_to_compute; i <= n; ++i) {
        seq_t sum_pos = (seq_t)0;
        seq_t sum_neg = (seq_t)0;
        for (idx_t k = 1; ; ++k) {
            idx_t pent1 = k * (3 * k - 1) / 2;
            idx_t pent2 = k * (3 * k + 1) / 2;
            if (pent1 > i && pent2 > i) break;

            seq_t term;
            if (pent1 <= i) {
                term = p_cache[i - pent1];
                if (k & 1) sum_pos += term;
                else       sum_neg += term;
            }
            if (pent2 <= i) {
                term = p_cache[i - pent2];
                if (k & 1) sum_pos += term;
                else       sum_neg += term;
            }
        }
        p_cache[i] = sum_pos - sum_neg;
    }
    next_to_compute = n + 1;          /* all indices up to n have been computed */

    /* a(n) = p(n) - floor(n/2) */
    seq_t floor_half = (seq_t)(n / 2);  /* integer division truncates down */
    return p_cache[n] - floor_half;
}
