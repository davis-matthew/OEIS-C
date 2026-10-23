/*
 * A049751  Array T read by diagonals; T(i,j)=number of directions from (0,0)
 *          to lattice points in closed disk with radius (0,0)-to-(i,j).
 * https://oeis.org/A049751
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 2 * F(i^2 + j^2) where (i,j) is the antidiagonal (i + j =
 *            s, 0 ≤ i,j ≤ s) corresponding to n, and F(r) is the number of
 *            primitive lattice vectors (x,y) with x≥0, y≥0, not both zero,
 *            and x² + y² ≤ r
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   int_sqrt_floor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/int_sqrt_floor.h"

static seq_t *f_cache = NULL;            /* prefix counts of primitive vectors   */
static idx_t f_cache_size = 0;            /* number of elements in f_cache       */

/* integer GCD for idx_t values */
static inline idx_t int_gcd(idx_t a, idx_t b)
{
    while (b) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Build prefix array f_cache up to rmax (inclusive) */
static void build_f_cache(idx_t rmax)
{
    idx_t sz = rmax + 1;
    f_cache = (seq_t *)realloc(f_cache, sz * sizeof(seq_t));
    if (!f_cache) abort();
    for (idx_t k = 0; k < sz; ++k) f_cache[k] = 0;

    idx_t max_x = int_sqrt_floor(rmax);
    for (idx_t x = 0; x <= max_x; ++x) {
        idx_t max_y = int_sqrt_floor(rmax - x * x);
        for (idx_t y = 0; y <= max_y; ++y) {
            if (x == 0 && y == 0) continue;
            if (int_gcd(x, y) == 1) {
                idx_t r = x * x + y * y;
                f_cache[r] += 1;
            }
        }
    }
    for (idx_t i = 1; i < sz; ++i)
        f_cache[i] += f_cache[i - 1];
    f_cache_size = sz;
}

/* Ensure f_cache can answer queries up to rmax */
static void ensure_f_cache(idx_t rmax)
{
    if (f_cache_size <= rmax)
        build_f_cache(rmax);
}

/* Compute a(n) according to the definition of A049751 */
seq_t a(idx_t n)
{
    /* Find s such that s(s+1)/2 <= n < (s+1)(s+2)/2 */
    idx_t s = 0;
    while ((s + 1) * (s + 2) / 2 <= n)
        ++s;

    idx_t k = n - s * (s + 1) / 2;
    idx_t i = s - k;
    idx_t j = k;

    idx_t r2 = i * i + j * j;
    ensure_f_cache(r2);

    seq_t res = (seq_t)2 * f_cache[r2];
    return res;
}
