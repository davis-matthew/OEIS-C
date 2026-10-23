/*
 * A170297  Number of reduced words of length n in Coxeter group on 48
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^41 = I.
 * https://oeis.org/A170297
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) satisfies the recurrence:
 *   a(0) = 1
 *   for 1 ≤ n ≤ 40:  a(n) = 46 * ∑_{i=0}^{n-1} a(i) + 2
 *   a(41) = 46 * ∑_{i=1}^{40} a(i) – 1081 * a(0) + 1
 *   for n ≥ 42:      a(n) = 46 * ∑_{i=n-40}^{n-1} a(i) – 1081 * a(n-41)
 * These coefficients come from the rational generating function of OEIS
 * sequence A170297.
 *
 * The implementation uses a simple cache so that repeated calls to a(n)
 * are O(1) after the first computation up to that n.  Each term is
 * computed directly from the recurrence above; intermediate sums are
 * taken in the type seq_t, allowing arbitrary‑precision evaluation
 * when seq_t is a big integer class.  For 64‑bit builds the
 * arithmetic is exact as long as the result fits in uint64_t.
 */

#include <stddef.h>
#include <stdlib.h>

/* static cache for computed values */
static seq_t *cache = NULL;     /* array of seq_t values */
static size_t cache_cap = 0;    /* capacity of the cache (>= cache_len) */
static idx_t cache_len = 0;     /* number of values already computed */

/* ensure the cache can hold at least n+1 values (indices 0..n) */
static void ensure_cap(idx_t n)
{
    if (cache_cap >= (size_t)n + 1)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 128;
    while (new_cap <= (size_t)n)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* compute a(n) using the recurrence above */
seq_t a(idx_t n)
{
    if (n < cache_len)          /* value already known */
        return cache[n];

    ensure_cap(n);

    for (idx_t i = cache_len; i <= n; ++i)
    {
        if (i == 0)
        {
            cache[0] = (seq_t)1;
            continue;
        }

        if (i <= 40)            /* 1 ≤ i ≤ 40 */
        {
            seq_t sum = 0;
            for (idx_t j = 0; j < i; ++j)
                sum += cache[j];
            cache[i] = (seq_t)46 * sum + (seq_t)2;
        }
        else if (i == 41)       /* special inhomogeneous term */
        {
            seq_t sum_1_to_40 = 0;
            for (idx_t j = 1; j <= 40; ++j)
                sum_1_to_40 += cache[j];
            cache[41] = (seq_t)46 * sum_1_to_40
                       - (seq_t)1081 * cache[0] + (seq_t)1;
        }
        else                    /* i ≥ 42: homogeneous recurrence */
        {
            seq_t sum_prev40 = 0;
            for (idx_t j = i - 40; j <= i - 1; ++j)
                sum_prev40 += cache[j];
            cache[i] = (seq_t)46 * sum_prev40
                       - (seq_t)1081 * cache[i - 41];
        }
    }

    cache_len = n + 1;
    return cache[n];
}
