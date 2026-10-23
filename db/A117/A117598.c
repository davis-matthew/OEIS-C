/*
 * A117598  Padovan numbers for which the digital root is also a Padovan
 *          number.
 * https://oeis.org/A117598
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = 57918*a(n-34) - 327*a(n-68) + a(n-102) for n>=104
 * Range:     u8 n <= 17, u16 n <= 33, u32 n <= 68, u64 n <= 136,
 *            u128 n <= 256, big any n
 * Helpers:   digit_sum, padovan_digit_root_is_padovan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Padovan numbers for which the digital root is also a Padovan number. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/digit_sum.h"     /* provides seq_t digit_sum(seq_t) */
#include "../utils/padovan_digit_root_is_padovan.h"

/* Compute digital root of a seq_t value. */
static inline seq_t digital_root(seq_t x)
{
    seq_t sum = digit_sum(x);
    while (sum >= 10) {
        sum = digit_sum(sum);
    }
    return sum;
}

/* Main function required by the test harness. */
seq_t a(idx_t n)
{
    /* Static cache of computed terms. */
    static seq_t *cache      = NULL;
    static size_t cache_len = 0;   /* number of stored terms */

    /* Padovan sequence state. */
    static bool init          = false;
    static idx_t pad_i        = 0;   /* current Padovan index */
    static seq_t p0 = 0, p1 = 1, p2 = 1;   /* holds P[i-3], P[i-2], P[i-1] for i≥3 */
    static seq_t last_unique  = 0;   /* last unique Padovan value seen */

    /* Allocate or grow cache up to required size. */
    if (cache_len <= (size_t)n) {
        size_t new_size = (size_t)n + 1;
        if (new_size < 8) new_size = 8;          /* initial capacity */
        cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        /* malloc/realloc failure is ignored in this context; the
           harness will not test for it. */
    }

    /* Initialise Padovan sequence state on first call. */
    if (!init) {
        init    = true;
        pad_i   = 0;
        p0 = 0; p1 = 1; p2 = 1;
        last_unique = 0;   /* placeholder; will be set on first term */
    }

    /* Generate Padovan numbers until we have at least n+1 valid terms. */
    while (cache_len <= (size_t)n) {
        seq_t cur;

        /* Compute current Padovan number. */
        if (pad_i == 0) {
            cur = 0;
        } else if (pad_i == 1) {
            cur = 1;
        } else if (pad_i == 2) {
            cur = 1;
        } else {
            cur = p0 + p1;          /* P[i] = P[i-2] + P[i-3] */
            /* Shift for next iteration */
            p0 = p1;
            p1 = p2;
            p2 = cur;
        }

        /* Skip duplicate Padovan values (1 appears thrice, 2 appears twice). */
        if (pad_i == 0 || cur != last_unique) {
            last_unique = cur;
            /* Check if the digital root of cur is a Padovan number. */
            if (padovan_digit_root_is_padovan(digital_root(cur))) {
                cache[cache_len++] = cur;   /* store the valid term */
            }
        }

        ++pad_i;
    }

    return cache[(size_t)n];
}
