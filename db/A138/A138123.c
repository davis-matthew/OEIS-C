/*
 * A138123  Antidiagonal sums of a triangle of coefficients of recurrences of
 *          the Fibonacci sequence.
 * https://oeis.org/A138123
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 46, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A138123: Antidiagonal sums of a triangle of coefficients of recurrences
 *  of the Fibonacci sequence.
 *
 *  Recurrence (for n>7):
 *      a(n) = a(n-2) - a(n-3) + a(n-4) + a(n-5) + a(n-7)
 *
 *  Initial values:
 *      a(1)=1, a(2)=1, a(3)=3, a(4)=0, a(5)=3, a(6)=0, a(7)=7
 *
 *  The function memoises already‑computed terms so that calls may be in any
 *  order.  It works for all idx_t ≥1 and for any seq_t as required by the
 *  build system.
 */

#include <stddef.h>
#include <stdlib.h>

/* External linkage as requested. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* behaviour for indices below the offset is unspecified */

    /* Ensure the cache is large enough for the requested index and for the
     * initial seven values.  The cache indexes are 1‑based; index 0 is unused
     * but allocated to keep pointer arithmetic simple. */
    size_t needed = (size_t)n + 1;
    if (needed < 8) needed = 8;      /* always keep room for a(1)…a(7) */

    static seq_t *cache    = NULL;   /* cached sequence values   */
    static size_t cache_cap = 0;      /* current allocated size   */
    static idx_t  cached_upto = 0;    /* largest index fully computed */

    if (needed > cache_cap) {
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);    /* allocation failure */
        /* Zero the newly added slots. */
        for (size_t i = cache_cap; i < needed; ++i) tmp[i] = 0;
        cache = tmp;
        cache_cap = needed;
    }

    /* Initialise the first seven terms (once). */
    if (cached_upto == 0) {
        static const seq_t init[8] = {0, 1, 1, 3, 0, 3, 0, 7};
        for (idx_t i = 1; i <= 7; ++i) cache[i] = init[i];
        cached_upto = 7;
    }

    /* Compute any missing terms up to index n. */
    if (cached_upto < n) {
        for (idx_t i = cached_upto + 1; i <= n; ++i) {
            /* All terms up to 7 are already set; avoid re‑computing them. */
            if (i <= 7) continue;
            /* a(i) = a(i-2) - a(i-3) + a(i-4) + a(i-5) + a(i-7) */
            seq_t val = cache[i - 2];
            val -= cache[i - 3];
            val += cache[i - 4];
            val += cache[i - 5];
            val += cache[i - 7];
            cache[i] = val;
        }
        cached_upto = n;
    }

    return cache[n];
}
