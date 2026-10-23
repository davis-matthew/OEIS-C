/*
 * A163986  Sum of all repeated parts of all partitions of n.
 * https://oeis.org/A163986
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = sum_{k=1}^{n} k * M_k(n) where M_k(n) = sum_{t=1}^{⌊n/k⌋}
 *            p(n − t*k), and we include the term only when M_k(n) ≥ 2. p(m)
 *            is the number of integer partitions of m
 * Range:     u8 n <= 9, u16 n <= 26, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *p_cache = NULL;   /* p_cache[i] holds p(i) */
static idx_t p_cache_size = 0;  /* number of computed values (p_cache_size = N+1) */

static void ensure_partitions(idx_t N)
{
    if (p_cache_size > N) return;                 /* already available */

    /* Grow the cache to fit indices 0..N */
    idx_t new_size = N + 1;
    seq_t *tmp = (seq_t *)realloc(p_cache, new_size * sizeof(seq_t));
    if (!tmp) abort();
    p_cache = tmp;

    /* Initialize p(0) */
    if (p_cache_size == 0) {
        p_cache[0] = (seq_t)1;
        p_cache_size = 1;
    }

    /* Compute missing partition numbers using Euler's pentagonal recurrence */
    for (idx_t i = p_cache_size; i <= N; ++i) {
        seq_t sum = 0;
        for (idx_t k = 1; ; ++k) {
            idx_t pent1 = k * (3 * k - 1) / 2;
            if (pent1 > i) break;
            if (k & 1)
                sum += p_cache[i - pent1];
            else
                sum -= p_cache[i - pent1];

            idx_t pent2 = k * (3 * k + 1) / 2;
            if (pent2 > i) continue;
            if (k & 1)
                sum += p_cache[i - pent2];
            else
                sum -= p_cache[i - pent2];
        }
        p_cache[i] = sum;
    }
    p_cache_size = new_size;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    ensure_partitions(n);

    seq_t result = 0;
    for (idx_t k = 1; k <= n; ++k) {
        idx_t tmax = n / k;
        seq_t m = 0;
        for (idx_t t = 1; t <= tmax; ++t)
            m += p_cache[n - t * k];
        if (m >= (seq_t)2)
            result += (seq_t)k * m;
    }
    return result;
}
