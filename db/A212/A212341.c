/*
 * A212341  Sequence of coefficients of x in marked mesh pattern generating
 *          function Q_{n,132}^(0,0,4,0)(x).
 * https://oeis.org/A212341
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 24, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A212341: coefficients of x in the marked mesh pattern generating function.
   Recurrence (valid for n ≥ 9):
     a(n) = 3 a(n-1) + a(n-3) + 3 a(n-4) – 33 a(n-5) – 14 a(n-6)
            – 12 a(n-7) + 12 a(n-8) + 143 a(n-9) + 135 a(n-10)
            + 150 a(n-11) + 125 a(n-12)
   Initial values (a(n) = 0 for n < 5):
     a(5)=14, a(6)=56, a(7)=188, a(8)=603,
     a(9)=1907, a(10)=5615, a(11)=15968, a(12)=44464
   The function memoises as needed and is safe for any call order. */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle indices below the offset.  Their values are irrelevant. */
    if (n < 5) {
        return 0;
    }

    /* Static cache of computed terms. */
    static seq_t *cache = NULL;
    static size_t cached = 0;

    /* First-time initialisation. */
    if (cache == NULL) {
        /* Store terms up to index 12; indices <5 are already 0. */
        cached = 13;                /* indices 0 … 12 */
        cache = (seq_t *)malloc(cached * sizeof(seq_t));
        if (!cache) return 0;        /* out of memory; give up */

        /* Initialise all entries to zero. */
        for (size_t i = 0; i < cached; ++i) {
            cache[i] = 0;
        }

        /* Place the known initial values. */
        cache[5]  = 14;
        cache[6]  = 56;
        cache[7]  = 188;
        cache[8]  = 603;
        cache[9]  = 1907;
        cache[10] = 5615;
        cache[11] = 15968;
        cache[12] = 44464;
    }

    /* Grow the cache if necessary. */
    if ((size_t)n >= cached) {
        size_t newc = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, newc * sizeof(seq_t));
        if (!tmp) return 0;               /* allocation failure */
        cache = tmp;
        /* Zero the newly added entries. */
        for (size_t i = cached; i < newc; ++i) cache[i] = 0;
        cached = newc;
    }

    /* Compute missing terms up to n using the recurrence. */
    for (size_t k = 13; k <= (size_t)n; ++k) {
        seq_t val = 0;
        val += cache[k - 1] * 3;
        val += cache[k - 3];
        val += cache[k - 4] * 3;
        val -= cache[k - 5] * 33;
        val -= cache[k - 6] * 14;
        val -= cache[k - 7] * 12;
        val += cache[k - 8] * 12;
        val += cache[k - 9] * 143;
        val += cache[k -10] * 135;
        val += cache[k -11] * 150;
        val += cache[k -12] * 125;
        cache[k] = val;
    }

    return cache[(size_t)n];
}
