/*
 * A162008  Third right hand column of the EG1 triangle A162005.
 * https://oeis.org/A162008
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (64*n^2+112*n+30-432*n*9^n-405*9^n+375*25^n)/128. The
 *            function uses a fast power routine for 128‑bit integers when the
 *            underlying seq_t type is built‑in (≤64 bits). For
 *            arbitrary‑precision seq_t types a linear recurrence with
 *            memoisation is used
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

/* Small‑int utilities ----------------------------------------------------- */
#if defined(__GNUC__) || defined(__clang__)
static unsigned __int128 pow128(unsigned __int128 base, idx_t exp) {
    unsigned __int128 res = 1;
    while (exp) {
        if (exp & 1) res *= base;
        base *= base;
        exp >>= 1;
    }
    return res;
}
#endif

/* Main function ----------------------------------------------------------- */
seq_t a(idx_t n) {
    /* Offset 1: a(1) is the first term. */
    if (n == 0) return 0;          /* undefined, but return 0 for safety */

    /* -------------------------------------------------------------
       Small seq_t types (≤64 bits).  Use 128‑bit arithmetic and a fast
       power routine to avoid overflow until the final shift.
       ------------------------------------------------------------- */
    if (sizeof(seq_t) <= 8) {
#if defined(__GNUC__) || defined(__clang__)
        unsigned __int128 n128 = (unsigned __int128)n;
        unsigned __int128 n2   = n128 * n128;

        unsigned __int128 pow9  = pow128((unsigned __int128)9,  n);
        unsigned __int128 pow25 = pow128((unsigned __int128)25, n);

        unsigned __int128 num = (unsigned __int128)64  * n2
                              + (unsigned __int128)112 * n128
                              + 30
                              - (unsigned __int128)432 * n128 * pow9
                              - (unsigned __int128)405 * pow9
                              + (unsigned __int128)375 * pow25;

        unsigned __int128 ans = num >> 7;                /* divide by 128 */
        return (seq_t)ans;
#else
        /* Fall back to the recurrence path if compilers lack __int128. */
#endif
    }

    /* -------------------------------------------------------------
       Arbitrary‑precision seq_t types.  Use the linear recurrence
       with memoisation to handle any index safely.
       ------------------------------------------------------------- */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static const seq_t base[6] = {
        (seq_t)16,        /* a(1) */
        (seq_t)1032,      /* a(2) */
        (seq_t)36096,     /* a(3) */
        (seq_t)1035088,   /* a(4) */
        (seq_t)27426960,  /* a(5) */
        (seq_t)702812568  /* a(6) */
    };

    if (n <= 6) return base[n-1];

    if (n > cache_len) {
        size_t new_len = (size_t)n;
        seq_t *new_cache = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
        if (!new_cache) {
            /* Allocation failed: abort or handle appropriately. */
            abort();
        }
        cache = new_cache;
        /* Initialise cache with the first six terms if not already set. */
        if (cache_len < 6) {
            for (size_t i = 0; i < 6; ++i) cache[i] = base[i];
        }
        /* Compute missing terms using the recurrence. */
        size_t start = (cache_len > 6) ? cache_len : 6;
        for (size_t i = start; i < new_len; ++i) {
            cache[i] = 46 * cache[i-1]
                     - 663 * cache[i-2]
                     + 3748 * cache[i-3]
                     - 7711 * cache[i-4]
                     + 6606 * cache[i-5]
                     - 2025 * cache[i-6];
        }
        cache_len = new_len;
    }
    return cache[n-1];
}
