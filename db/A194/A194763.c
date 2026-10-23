/*
 * A194763  Number of k < n such that {k*2^(1/3)} > {n*2^(1/3)}, where { } =
 *          fractional part.
 * https://oeis.org/A194763
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) counts k < n such that {k*2^(1/3)} > {n*2^(1/3)}. The
 *            implementation uses integer cube roots to compute
 *            floor(n·2^(1/3)) and a scaling factor equal to the current
 *            maximum index squared. Fractional parts are represented as
 *            integers f[n] = floor(n·M·2^(1/3)) – M·floor(n·2^(1/3)), which
 *            preserv...
 * Range:     u8 n <= 16, u16 n <= 1177, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* --------------------------------------------------------------
   integer cube root: returns the largest integer x s.t. x³ <= v
   v is a 128‑bit unsigned integer.
   -------------------------------------------------------------- */
static inline uint64_t cube_root_floor(__uint128_t v)
{
    /* Upper bound: for v up to 2·(1e12)³ ≈ 2e36, the cube root
       is < 2.2·10¹², comfortably below 1ULL<<41.                */
    uint64_t low = 0;
    uint64_t high = 1ULL << 41;   /* ≈ 2.2·10¹², covers all needed cases */

    while (low < high) {
        uint64_t mid = (low + high + 1) >> 1;
        __uint128_t m3 = (__uint128_t)mid * mid * mid;
        if (m3 <= v)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* --------------------------------------------------------------
   Global cache for scaled fractional parts.
   f_cache[n] holds f[n] as described above.
   -------------------------------------------------------------- */
static seq_t *f_cache = NULL;
static idx_t  cache_size = 0;      /* number of pre‑computed indices */
static uint64_t M_factor = 1;     /* current scaling factor (M = n²) */

/* --------------------------------------------------------------
   Compute sequence value a(n).
   -------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* sequence is defined for n >= 1 */

    /* If we have not yet computed up to n, extend the cache.  */
    if (n > cache_size) {
        idx_t new_size = n;
        uint64_t new_M  = (uint64_t)new_size * new_size;

        /* Allocate a new array (size+1 to keep 1‑based indexing).  */
        seq_t *new_f = (seq_t *)realloc(f_cache, (new_size + 1) * sizeof(seq_t));
        if (!new_f)
            return 0; /* allocation failed; return 0 as safe default */

        f_cache   = new_f;
        cache_size = new_size;
        M_factor   = new_M;

        /* Recompute all f[k] with the new scaling factor.  */
        for (idx_t k = 1; k <= new_size; ++k) {

            /* floor(k·α) where α = 2^(1/3) */
            __uint128_t k3    = (__uint128_t)k * k * k;
            uint64_t a_k      = cube_root_floor(2 * k3);

            /* floor((k·M)·α) */
            __uint128_t Mk    = (__uint128_t)new_M * k;
            __uint128_t Mk3   = Mk * Mk * Mk;
            uint64_t b_k      = cube_root_floor(2 * Mk3);

            /* scaled fractional part: floor(M·{k·α}) = b_k – M·a_k */
            uint64_t f_k      = b_k - new_M * a_k;
            f_cache[k]        = (seq_t)f_k;
        }
    }

    /* Count how many previous indices have a larger scaled fractional part. */
    seq_t count = 0;
    seq_t f_n   = f_cache[n];
    for (idx_t i = 1; i < n; ++i)
        if (f_cache[i] > f_n)
            ++count;

    return count;
}
