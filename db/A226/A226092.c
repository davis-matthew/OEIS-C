/*
 * A226092  Fourth powers that become prime when their most significant
 *          (leftmost) decimal digit is removed.
 * https://oeis.org/A226092
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 19, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   digit_len, isPrimeGeneric, pow10, pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS A226092: the n-th fourth power k⁴ whose most
   significant decimal digit is removed leaving a prime number.
   The function searches successive integers k, checks the condition,
   and memoises the found values so that any call order is fast. */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

/* Helper function prototypes supplied by the build environment */
#include "../utils/pow_int.h"
#include "../utils/digit_len.h"
#include "../utils/pow10.h"
#include "../utils/isPrimeGeneric.h"

/* Cached sequence values ------------------------------------------- */

/* Pointer to the cached array */
static seq_t *seq_cache = NULL;
/* Number of terms currently stored */
static size_t cache_len = 0;
/* Current allocated capacity of seq_cache */
static size_t cache_cap = 0;
/* The next integer k that still needs to be examined */
static idx_t next_k = 1;

/* Ensure that seq_cache can hold at least 'need' elements */
static void ensure_cache_capacity(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < need) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    seq_cache = tmp;
    cache_cap = new_cap;
}

/* Return the n-th term of the sequence (n >= 1) */
seq_t a(idx_t n)
{
    /* Undefined for offset 0, but the caller should never request it */
    if (n == 0) return 0;

    /* Grow the cache until it contains at least n terms */
    while (cache_len < (size_t)n)
    {
        /* Compute k⁴ */
        seq_t p = pow_int((seq_t)next_k, 4);

        /* Number of decimal digits of p */
        int d = digit_len(p);
        if (d > 0)
        {
            /* Remove the most significant digit: keep the last d-1 digits */
            seq_t mod = pow10((idx_t)(d - 1));
            seq_t r = p % mod;

            if (isPrimeGeneric(r))
            {
                /* Store p, expanding the cache if necessary */
                ensure_cache_capacity(cache_len + 1);
                seq_cache[cache_len++] = p;
            }
        }

        ++next_k;
    }

    return seq_cache[(size_t)n - 1];
}
