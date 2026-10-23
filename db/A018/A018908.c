/*
 * A018908  Define sequence S(a_0,a_1) by a_{n+2} is least integer such that
 *          a_{n+2}/a_{n+1}>a_{n+1}/a_n for n >= 0. This is S(3,4).
 * https://oeis.org/A018908
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 18, u32 n <= 38, u64 n <= 77, u128 n <= 155,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence S(3,4):  a(0)=3, a(1)=4,
   for n ≥ 0  a(n+2) = ⌊a(n+1)² / a(n)⌋ + 1
   (minimal integer making a(n+2)/a(n+1) > a(n+1)/a(n)). */

#include <stdlib.h>
#include <stdint.h>

/* Helper: compute the next term from the two previous ones.
   Works for both built‑in unsigned integer types (using 128‑bit)
   and for arbitrary‑precision classes (C++ overloads). */
static inline seq_t compute_next(const seq_t a_nm1, const seq_t a_n)
{
#if defined(__cplusplus)
    /* C++: use arbitrary‑precision arithmetic provided by seq_t. */
    return (a_nm1 * a_nm1) / a_n + 1;
#else
    /* C: use unsigned __int128 to avoid overflow when seq_t is uint64_t. */
#if defined(__SIZEOF_INT128__)
    unsigned __int128 prod = (unsigned __int128)a_nm1 * a_nm1;
    return (seq_t)(prod / (unsigned __int128)a_n) + 1;
#else
    /* Fallback: no 128‑bit support.  This will overflow for large n
       but the 64‑bit build is typically compiled with a compiler that
       defines __int128. */
    return (seq_t)((a_nm1 * a_nm1) / a_n) + 1;
#endif
#endif
}

seq_t a(idx_t n)
{
    /* Static cache for memoisation. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* Initialise cache with the first two terms. */
    if (cache == NULL) {
        cache_cap = 8;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;            /* allocation failed */
        cache[0] = (seq_t)3;
        cache[1] = (seq_t)4;
        cache_len = 2;
    }

    /* Grow cache to include a(n). */
    if ((size_t)n >= cache_len) {
        size_t new_len = (size_t)n + 1;
        if (new_len > cache_cap) {
            while (cache_cap < new_len)
                cache_cap <<= 1;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            if (!cache) return 0;          /* allocation failed */
        }

        /* Compute missing terms. */
        for (size_t i = cache_len; i < new_len; ++i) {
            /* a(i-2) = cache[i-2], a(i-1) = cache[i-1] */
            cache[i] = compute_next(cache[i - 1], cache[i - 2]);
        }
        cache_len = new_len;
    }

    return cache[(size_t)n];
}
