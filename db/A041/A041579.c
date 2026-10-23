/*
 * A041579  Denominators of continued fraction convergents to sqrt(307).
 * https://oeis.org/A041579
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 177058564*a(n-14) - a(n-28) for n > 27 Initial values:
 *            a(0)=1, a(1)=1, a(2)=2, a(3)=23, a(4)=117, a(5)=140, a(6)=537,
 *            a(7)=9269, a(8)=28344, a(9)=37613, a(10)=216409, a(11)=2418112,
 *            a(12)=2634521, a(13)=5052633, a(14)=174424043, a(15)=179476676,
 *            a(16)=353900719, a(17)=4072384585, a(18)=207...
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 34, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* seq_t and idx_t are supplied by the build environment. */
seq_t a(idx_t n)
{
    /* Cache for computed terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_size = 0;   /* number of terms stored (cache[0]..cache[cache_size-1]) */

    /* -------------------------------------------------------------
       Initialize the first 28 terms using the continued–fraction
       expansion of sqrt(307). This is a one–time cost performed on
       the first call, regardless of the value of n.
       ------------------------------------------------------------- */
    if (cache_size == 0) {
        /* Allocate enough room for 28 terms plus a little extra. */
        cache_cap  = 32;
        cache_size = 28;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;  /* allocation failure – undefined in tests */

        /* Continued–fraction expansion of sqrt(307). */
        const uint64_t D  = 307ULL;
        const uint64_t a0 = 17ULL;            /* floor(sqrt(307)) */

        /* Convergents: h[-2] = 0, h[-1] = 1,
           k[-2] = 1, k[-1] = 0.  We keep only the denominators. */
        seq_t k_prev_prev = 1;   /* k[-2]  */
        seq_t k_prev      = 0;   /* k[-1]  */

        /* First partial quotient produces k[0] = 1. */
        seq_t k = a0 * k_prev + k_prev_prev;
        cache[0] = k;
        k_prev_prev = k_prev;
        k_prev      = k;

        /* The continued fraction of sqrt(D) eventually repeats with
           period 14.  We generate the next 27 partial quotients. */
        uint64_t m       = 0;
        uint64_t d       = 1;
        uint64_t a_prev  = a0;
        for (size_t i = 1; i < 28; ++i) {
            m     = d * a_prev - m;
            d     = (D - m * m) / d;
            uint64_t a = (a0 + m) / d;

            k = (seq_t)a * k_prev + k_prev_prev;
            cache[i] = k;
            k_prev_prev = k_prev;
            k_prev      = k;
            a_prev      = a;
        }
    }

    /* -------------------------------------------------------------
       Return cached value if already known.
       ------------------------------------------------------------- */
    if ((size_t)n < cache_size)
        return cache[n];

    /* -------------------------------------------------------------
       Grow the cache to at least n+1 entries, computing any new
       terms via the recurrence.
       ------------------------------------------------------------- */
    size_t needed = (size_t)n + 1;
    if (needed > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2U : 1U;
        while (new_cap < needed)
            new_cap *= 2U;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!cache) return 0;                  /* undefined on failure */
        cache_cap = new_cap;
    }

    /* Compute the missing terms.  The recurrence only applies for i >= 28. */
    for (size_t i = cache_size; i < needed; ++i) {
        /* For i < 28 everything is already in cache. */
        if (i < 28)
            continue;

        idx_t idx1 = (idx_t)i - 14;
        idx_t idx2 = (idx_t)i - 28;
        cache[i] = cache[idx1] * (seq_t)177058564ULL - cache[idx2];
    }

    cache_size = needed;
    return cache[n];
}
