/*
 * A217689  a(1)=2, a(2)=3, a(3)=4; for n>=4, a(n) is the largest number <=
 *          prime(n) such that no terms of the sequence are between a(n-1)/2
 *          and a(n)/2.
 * https://oeis.org/A217689
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=2, a(2)=3, a(3)=4. For n ≥ 4, let k be the largest index
 *            with a(k) ≤ a(n‑1)/2 < a(k+1). Then a(n) = 2*a(k+1) if prime(n)
 *            > 2*a(k+1) = prime(n) otherwise
 * Range:     u8 n <= 54, u16 n <= 6541, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/get_prime.h"

static seq_t *seq = NULL;          /* dynamic array of terms, 1‑based */
static idx_t max_computed = 0;     /* largest index already stored */
static idx_t cache_cap = 0;        /* current capacity of seq array */

/* enlarge cache to hold at least indices 1 … n */
static void ensure_cache(idx_t n)
{
    if (cache_cap >= n + 1) return;
    idx_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < n + 1) new_cap *= 2;
    seq = (seq_t *)realloc(seq, sizeof(seq_t) * new_cap);
    cache_cap = new_cap;
}

/* binary search for largest k < limit with seq[k] ≤ half */
static idx_t find_k(idx_t limit, seq_t half)
{
    idx_t lo = 1, hi = limit - 1, best = 0;
    while (lo <= hi) {
        idx_t mid = (lo + hi) >> 1;
        if (seq[mid] <= half) {
            best = mid;
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return best;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n==0 */
    ensure_cache(n);

    /* initialise the first three terms on first call */
    if (max_computed == 0) {
        seq[1] = 2;
        seq[2] = 3;
        seq[3] = 4;
        max_computed = 3;
    }

    /* compute any missing terms up to n */
    if (n > max_computed) {
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            seq_t half = seq[i - 1] / 2;
            idx_t k = find_k(i, half);                /* a(k) ≤ half < a(k+1) */
            seq_t candidate = 2 * seq[k + 1];          /* 2*a(k+1) */
            seq_t prime_val = (seq_t)get_prime(i);    /* prime(i) */
            seq[i] = (prime_val > candidate) ? candidate : prime_val;
        }
        max_computed = n;
    }
    return seq[n];
}
