/*
 * A164665  Number of reduced words of length n in Coxeter group on 29
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^7 = I.
 * https://oeis.org/A164665
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the linear recurrence a(n) =
 *            27*(a(n-1)+a(n-2)+a(n-3)+a(n-4)+a(n-5)+a(n-6)) - 378*a(n-7) for
 *            n ≥ 8. The first 7 values are given, and a(7) contains an
 *            additional +1 due to the numerator of the generating function
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses a static cache to memoise values.  It
   works for any seq_t type provided by the build (uint8_t, uint16_t,
   uint32_t, uint64_t, unsigned __int128, or arbitrary‑precision
   integer).  All arithmetic uses 64‑bit temporaries where possible
   to avoid overflow when seq_t is 32‑bit or smaller. */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    size_t idx = (size_t)n;           /* safe for n within 10 000 */

    /* Initialise cache with the first 8 terms. */
    if (cap == 0) {
        cap   = 8;                       /* indices 0 … 7 */
        cache = (seq_t *)calloc(cap, sizeof(seq_t));
        if (!cache) return (seq_t)0;      /* allocation failure */

        cache[0] = (seq_t)1;
        cache[1] = (seq_t)29;
        cache[2] = (seq_t)812;
        cache[3] = (seq_t)22736;
        cache[4] = (seq_t)636608;
        cache[5] = (seq_t)17825024;
        cache[6] = (seq_t)499100672;

        /* a(7) contains an extra +1 coming from the numerator. */
        cache[7] = (seq_t)(
            27ULL*cache[6] + 27ULL*cache[5] + 27ULL*cache[4] +
            27ULL*cache[3] + 27ULL*cache[2] + 27ULL*cache[1] -
            378ULL*cache[0] + 1ULL);
    }

    /* Grow cache if needed. */
    if (idx + 1 > cap) {
        size_t old_cap = cap;
        size_t new_cap = idx + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* allocation failure */
        cache = tmp;
        cap   = new_cap;

        /* Compute new terms using the recurrence. */
        for (size_t i = old_cap; i <= idx; ++i) {
            /* For i == 7 we already have the special value. */
            if (i == 7) continue;

            seq_t val = (seq_t)(
                27ULL*cache[i-1] + 27ULL*cache[i-2] + 27ULL*cache[i-3] +
                27ULL*cache[i-4] + 27ULL*cache[i-5] + 27ULL*cache[i-6] -
                378ULL*cache[i-7]);

            cache[i] = val;
        }
    }

    return cache[idx];
}
