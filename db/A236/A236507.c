/*
 * A236507  Smallest k that is a concatenation of two numbers x and y where
 *          x^2 + y^2 = n^2.
 * https://oeis.org/A236507
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   a(n) = smallest k that is a concatenation of x and y where x² +
 *            y² = n² and the number of decimal digits of y is at most that of
 *            x (y is padded with leading zeros to match the length of x).
 *            This is exactly the definition of OEIS A236507
 * Range:     u8 n <= 10, u16 n <= 102, u32 n <= 500, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Integer square root for 64‑bit values (returns floor(sqrt(v))) */
static inline uint64_t isqrt_u64(uint64_t v)
{
    uint64_t low = 0, high = v;
    while (low <= high) {
        uint64_t mid = (low + high) >> 1;
        uint64_t sq  = mid * mid;
        if (sq == v) return mid;
        if (sq < v)  low = mid + 1;
        else         high = mid - 1;
    }
    return high;
}

/* Number of decimal digits of a positive integer; 0 → 1 digit */
static inline int digits_idx(idx_t val)
{
    if (val == 0) return 1;
    int d = 0;
    while (val) { val /= 10; ++d; }
    return d;
}

/* Compute a(n) for a single n (n >= 1).  Returns 0 if no admissible pair exists. */
static inline seq_t compute_a_for(idx_t n)
{
    seq_t best = 0;
    bool have  = false;
    idx_t nsq = n * n;

    for (idx_t x = 1; x <= n; ++x) {
        idx_t t = nsq - x * x;
        uint64_t y = isqrt_u64(t);
        if (y * y != t) continue;           /* not a perfect square */

        /* Disallow the trivial (y = 0) when n ≡ 0 (mod 5). */
        if (y == 0 && (n % 5 == 0)) continue;

        int dx = digits_idx(x);
        int dy = digits_idx((idx_t)y);
        if (dy > dx) continue;              /* y must not have more digits than x */

        /* Concatenate x and y in decimal, padding y to dx digits. */
        seq_t pow10 = 1;
        for (int i = 0; i < dx; ++i) pow10 *= (seq_t)10;

        seq_t k = ((seq_t)x) * pow10 + ((seq_t)y);
        if (!have || k < best) { best = k; have = true; }
    }
    return best;
}

/* Memoisation cache. */
static seq_t *cache    = NULL;
static idx_t  cache_sz = 0;

/* External linkage as required. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for n < 1 */

    if (n > cache_sz) {
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)n * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        for (idx_t i = cache_sz + 1; i <= n; ++i)
            cache[i - 1] = compute_a_for(i);
        cache_sz = n;
    }
    return cache[n - 1];
}
