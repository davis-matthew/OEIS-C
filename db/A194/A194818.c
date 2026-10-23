/*
 * A194818  Number of integers k in [1,n] such that {n*r+k*r} > {n*r-k*r},
 *          where { } = fractional part and r=sqrt(3).
 * https://oeis.org/A194818
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Sequence a(n): number of k with 1 ≤ k ≤ n such that
 *   { (n+k)*√3 } > { (n-k)*√3 }   (fractional part).
 * The function uses exact integer arithmetic only.
 */

/* Integer square‑root of a 128‑bit unsigned integer (floor). */
static uint64_t isqrt_uint128(unsigned __int128 x)
{
    unsigned __int128 low = 0;
    unsigned __int128 high = (unsigned __int128)1 << 64; /* exclusive upper bound */
    while (low < high) {
        unsigned __int128 mid = (low + high) >> 1;
        if (mid * mid <= x)
            low = mid + 1;
        else
            high = mid;
    }
    return (uint64_t)(low - 1);
}

/* Compute floor( sqrt(3) * m ) for an integer m.  Uses
 *   floor_sqrt( 3*m*m ).
 */
static uint64_t floor_sqrt3_times(uint64_t m)
{
    unsigned __int128 square = (unsigned __int128)3 * m * m;
    return isqrt_uint128(square);
}

/* Main sequence function.  Uses a static cache of
 *   floor_sqrt3_times(i) for i up to 2*max_n_seen.
 */
seq_t a(idx_t n)
{
    /* static cache data */
    static idx_t cached_max_n = 0;      /* largest n for which cache is ready */
    static uint64_t *fs_cache = NULL;  /* array size 2*cached_max_n + 1 */
    static size_t cache_size = 0;       /* current allocated size */

    /* Ensure cache large enough for indices up to 2*n. */
    idx_t needed = 2 * n;
    if (needed + 1 > (idx_t)cache_size) {
        size_t new_size = (size_t)needed + 1;
        void *p = realloc(fs_cache, new_size * sizeof(uint64_t));
        if (!p) return 0; /* allocation failure; caller should handle */
        fs_cache = (uint64_t *)p;
        cache_size = new_size;

        /* Compute new entries from old_max+1 up to needed. */
        idx_t start = cached_max_n + 1;
        for (idx_t i = start; i <= needed; ++i) {
            fs_cache[i] = floor_sqrt3_times((uint64_t)i);
        }
        cached_max_n = needed;
    }

    /* Count k satisfying the inequality. */
    seq_t count = 0;
    for (idx_t k = 1; k <= n; ++k) {
        /* floor_sqrt3_times(n+k)  -  floor_sqrt3_times(n-k)  < 2k√3   */
        uint64_t m1 = fs_cache[n + k];
        uint64_t m2 = fs_cache[n - k];
        uint64_t diff = m1 - m2;                /* non‑negative */
        unsigned __int128 diff_sq = (unsigned __int128)diff * diff;
        unsigned __int128 rhs    = (unsigned __int128)12 * k * k; /* (2k√3)^2 = 12 k^2 */

        if (diff_sq < rhs)
            ++count;
    }

    return (seq_t)count;
}
