/*
 * A194245  [sum{(rk) : 1<=k<=n}], where [ ]=floor, ( )=fractional part, and
 *          r=2-sqrt(3).
 * https://oeis.org/A194245
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( sum_{k=1}^n frac((2-sqrt3)k) )
 * Range:     u8 n <= 22, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation derived from the identities: */
/*   floor((2-sqrt3)k) = 2k - floor(k*sqrt3) - 1               */
/*   sum frac((2-sqrt3)k) = n + sum_{k=1}^n floor(k*sqrt3) - sqrt3*n(n+1)/2 */
/*   floor(sqrt3 * N) = floor(sqrt(3*N*N))                         */
/* Therefore a(n) = n + Σ_{k=1}^n floor(k*sqrt3) - floor(sqrt3 * n(n+1)/2) - 1 */

#include <stdlib.h>
#include <stdint.h>
#include <math.h>

/* Integer square root for 64‑bit unsigned values */
static inline uint64_t floor_sqrt_uint64(uint64_t x)
{
    uint64_t r = sqrtl((long double)x);
    while ((r + 1) * (r + 1) <= x) ++r;
    while (r * r > x) --r;
    return r;
}

/* Global cache for cumulative sums of floor(k*sqrt3) */
static seq_t *cum_floor_sqrt3 = NULL;
static idx_t cum_cache_size = 0;

/* Ensure cache holds data up to index n */
static void ensure_cache(idx_t n)
{
    if (n <= cum_cache_size) return;
    /* Allocate new buffer (size n+1 to have index 0..n) */
    seq_t *new_cache = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!new_cache) exit(EXIT_FAILURE);
    /* Initialise from previous cache */
    for (idx_t i = 0; i <= cum_cache_size; ++i)
        new_cache[i] = cum_floor_sqrt3 ? cum_floor_sqrt3[i] : 0;
    /* Compute new entries */
    for (idx_t k = cum_cache_size + 1; k <= n; ++k)
    {
        /* term = floor(k * sqrt3) = floor(sqrt(3*k*k)) */
        unsigned __int128 tmp = (unsigned __int128)3 * (unsigned __int128)k * (unsigned __int128)k;
        uint64_t term = floor_sqrt_uint64((uint64_t)tmp);
        new_cache[k] = new_cache[k - 1] + (seq_t)term;
    }
    /* Free old cache */
    free(cum_floor_sqrt3);
    cum_floor_sqrt3 = new_cache;
    cum_cache_size = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for offset 1, but return 0 */

    /* Precompute cumulative sums up to n */
    ensure_cache(n);

    /* B = n*(n+1)/2 as seq_t */
    seq_t B = ((seq_t)n * (n + 1)) / 2;

    /* floorA_B = floor(sqrt3 * B) = floor(sqrt(3*B*B)) */
    unsigned __int128 prod = (unsigned __int128)3 * (unsigned __int128)B * (unsigned __int128)B;
    uint64_t floorA_B = floor_sqrt_uint64((uint64_t)prod);

    /* I = n + cum_floor_sqrt3[n] */
    seq_t I = (seq_t)n + cum_floor_sqrt3[n];

    /* a(n) = I - floorA_B - 1 */
    return I - (seq_t)floorA_B - 1;
}
