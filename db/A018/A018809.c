/*
 * A018809  Number of lines through exactly 2 points of an n X n grid of
 *          points.
 * https://oeis.org/A018809
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1/2 * (f(n,3) - 2*f(n,2) + f(n,1)) where f(n,k) =
 *            sum_{|x|<n, |y|<n, gcd(|x|,|y|)=1} (n - |k*x|)*(n - |k*y|)
 * Range:     u8 n <= 4, u16 n <= 18, u32 n <= 329, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function a(n) returns the nth term of A018809. */

#include <stdint.h>
#include <stdlib.h>

/* Basic gcd for unsigned integers                           */
static inline idx_t gcd_uint64(idx_t a, idx_t b)
{
    while (b != 0) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Cache for computed values                                 */
static seq_t *cache_vals = NULL;
static unsigned char *cache_flag = NULL;   /* 0 = not computed, 1 = computed */
static size_t cache_cap = 0;

static void cache_ensure(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap? cache_cap*2 : 1;
    while (new_cap < need) new_cap <<= 1;
    cache_vals = (seq_t*)realloc(cache_vals, new_cap * sizeof(seq_t));
    cache_flag = (unsigned char*)realloc(cache_flag, new_cap);
    for (size_t i = cache_cap; i < new_cap; ++i) cache_flag[i] = 0;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* a(0) = a(1) = 0 */
    if (n < 2) return 0;

    /* Ensure cache big enough */
    cache_ensure((size_t)n + 1);
    if (cache_flag[n]) return cache_vals[n];

    /* Work with integer types for loops; n <= 10000 */
    int N = (int)n;

    seq_t f[4] = {0, 0, 0, 0};

    /* Iterate over unordered pairs of lattice points */
    for (int dx = 0; dx < N; ++dx) {
        for (int dy = 0; dy < N; ++dy) {
            if (dx == 0 && dy == 0) continue;
            if (gcd_uint64(dx, dy) != 1) continue;       /* primitive vector */

            int mult = (dx == 0 || dy == 0) ? 2 : 4;     /* sign multiplicity */

            for (int k = 1; k <= 3; ++k) {
                if (k * dx >= N || k * dy >= N) continue;

                seq_t term = (seq_t)(N - k * dx) * (seq_t)(N - k * dy);
                f[k] += term * mult;
            }
        }
    }

    /* a(n) = (f3 - 2*f2 + f1) / 2   */
    seq_t ans = (f[3] + f[1] - (seq_t)2 * f[2]) / (seq_t)2;

    cache_vals[n] = ans;
    cache_flag[n] = 1;
    return ans;
}
