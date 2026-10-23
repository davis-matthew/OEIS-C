/*
 * A192256  0-sequence of reduction of (n^3) by x^2 -> x+1.
 * https://oeis.org/A192256
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 27, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 0-sequence of reduction of (n³) by x² → x+1
 *
 * The sequence satisfies the linear recurrence of order 9 with
 * coefficients derived from the empirical generating function
 * G(x) = x*(1-4x+29x²-36x³+43x⁴-16x⁵+2x⁶) / ((1-x)(1-x-x²)⁴)
 *
 * Recurrence (for n ≥ 10):
 *    a(n) = 5 a(n‑1) - 6 a(n‑2) - 6 a(n‑3) + 13 a(n‑4) + 3 a(n‑5)
 *                      - 10 a(n‑6) - 2 a(n‑7) + 3 a(n‑8) + a(n‑9)
 *
 * Initial values (n = 1…9):
 *    a = {1, 1, 28, 92, 342, 990, 2705, 6801, 16278}
 *
 * The implementation memoises already-computed terms so that
 * calls to a(n) may occur in any order.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

static seq_t *cache = NULL;      /* memoised terms, 0‑based indexing */
static size_t  capacity = 0;     /* allocated size of cache */
static size_t  computed = 0;     /* number of terms currently stored */

/* Initial nine terms (offset 1 => a(1) at index 0) */
static const seq_t init9[9] = {
    (seq_t)1,   (seq_t)1,   (seq_t)28,   (seq_t)92,
    (seq_t)342, (seq_t)990, (seq_t)2705, (seq_t)6801,
    (seq_t)16278
};

/* Ensure cache contains terms up to and including n (1‑based). */
static void ensure_cache(idx_t n)
{
    if (n <= computed) return;          /* already available */

    /* Allocate at least n elements plus some spare space */
    if (n > capacity) {
        size_t new_cap = n > 100 ? n : 100;   /* small grow factor */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();   /* allocation failure */
        cache = tmp;
        capacity = new_cap;
    }

    /* Initialise the first nine terms if not yet done */
    if (computed == 0) {
        for (size_t i = 0; i < 9; ++i) cache[i] = init9[i];
        computed = 9;
    }

    /* Compute new terms consecutively */
    for (idx_t k = (idx_t)computed + 1; k <= n; ++k) {
        size_t i = (size_t)(k - 1);       /* 0‑based index of a(k) */

        /* For k ≥ 10 we use the recurrence; otherwise already set */
        if (i >= 9) {
            /* Compute positive and negative contributions separately
             * to guarantee non‑negative subtraction.
             */
#if defined(__SIZEOF_INT128__)
// Use 128‑bit intermediate when available to avoid overflow in 64‑bit builds
            unsigned __int128 pos = 0;
            unsigned __int128 neg = 0;
            pos += (unsigned __int128)cache[i-1] * (unsigned __int128)5U;
            pos += (unsigned __int128)cache[i-4] * (unsigned __int128)13U;
            pos += (unsigned __int128)cache[i-5] * (unsigned __int128)3U;
            pos += (unsigned __int128)cache[i-8] * (unsigned __int128)3U;
            pos += (unsigned __int128)cache[i-9];                 /* *1 */

            neg += (unsigned __int128)cache[i-2] * (unsigned __int128)6U;
            neg += (unsigned __int128)cache[i-3] * (unsigned __int128)6U;
            neg += (unsigned __int128)cache[i-6] * (unsigned __int128)10U;
            neg += (unsigned __int128)cache[i-7] * (unsigned __int128)2U;

            cache[i] = (seq_t)(pos - neg);
#else
            /* Pure 64‑bit (or arbitrary‑precision) calculation.
             * Assumes pos ≥ neg for all n in the tested range.
             */
            seq_t pos = 0;
            seq_t neg = 0;
            pos = pos + cache[i-1] * (seq_t)5U;
            pos = pos + cache[i-4] * (seq_t)13U;
            pos = pos + cache[i-5] * (seq_t)3U;
            pos = pos + cache[i-8] * (seq_t)3U;
            pos = pos + cache[i-9];        /* *1 */

            neg = neg + cache[i-2] * (seq_t)6U;
            neg = neg + cache[i-3] * (seq_t)6U;
            neg = neg + cache[i-6] * (seq_t)10U;
            neg = neg + cache[i-7] * (seq_t)2U;

            cache[i] = pos - neg;          /* pos ≥ neg by design */
#endif
        }
    }
    computed = n;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    ensure_cache(n);
    return cache[(size_t)(n - 1)];
}
