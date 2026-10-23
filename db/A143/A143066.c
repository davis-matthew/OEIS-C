/*
 * A143066  Expansion of phi(x^3) / psi(x) in powers of x where phi(), psi()
 *          are Ramanujan theta functions.
 * https://oeis.org/A143066
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient of x^n in φ(x³)/ψ(x) Recurrence: a(0)=1; for
 *            n>0 a(n) = A(n) – Σ_{t triangular, 1≤t≤n} a(n‑t), where A(n)=2
 *            if n = 3·k² for some integer k≥1, otherwise A(n)=0 (and A(0)=1).
 *            Triangular numbers t satisfy t = k(k+1)/2
 * Range:     i8 n <= 48, i16 n <= 144, i32 n <= 458, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

static seq_t *cache = NULL;      /* memoised a(k) values */
static size_t cache_cap = 0;     /* allocated capacity */
static size_t cache_len = 0;     /* number of elements filled */

/* integer floor square root of a 64‑bit unsigned integer */
static idx_t isqrt_uint64(idx_t n)
{
    idx_t low = 0, high = n, ans = 0;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        if (mid == 0) { low = mid + 1; continue; }
        if (mid <= n / mid) { ans = mid; low = mid + 1; }
        else high = mid - 1;
    }
    return ans;
}

/* test whether n = 3·k² for some integer k≥1 */
static bool is_three_times_square(idx_t n)
{
    if (n == 0) return false;
    if (n % 3 != 0) return false;
    idx_t y = n / 3;                 /* y = k² */
    idx_t k = isqrt_uint64(y);
    if (k == 0) return false;
    return (y % k == 0) && (y / k == k);
}

seq_t a(idx_t n)
{
    /* expand cache if necessary */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap <= (size_t)n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* initialise a(0) if not yet done */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        cache_len = 1;
    }

    /* compute missing terms up to n */
    for (size_t m = cache_len; m <= (size_t)n; ++m) {
        seq_t a_m = (m == 0) ? (seq_t)1
                             : (is_three_times_square((idx_t)m) ? (seq_t)2 : (seq_t)0);

        seq_t sum = 0;
        for (idx_t k = 1; ; ++k) {
            /* t = k(k+1)/2 */
            unsigned __int128 prod = (unsigned __int128)k * (k + 1);
            idx_t t = (idx_t)(prod / 2);
            if (t > m) break;
            sum += cache[m - t];
        }
        cache[m] = a_m - sum;
    }

    cache_len = (size_t)n + 1;
    return cache[(size_t)n];
}
