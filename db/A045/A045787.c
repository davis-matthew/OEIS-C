/*
 * A045787  Squares with initial digit '4'.
 * https://oeis.org/A045787
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A045858(n)^2
 * Range:     u8 n <= 2, u16 n <= 36, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   digit_len, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdlib.h>
#include <limits.h>

/* Utility functions supplied by the build. */
#include "../utils/digit_len.h"    /* int digit_len(seq_t x);         */
#include "../utils/pow10.h"       /* seq_t pow10(idx_t e);          */

/* Cached squares of the required numbers. */
static seq_t *cache_sq   = NULL;   /* array of squares          */
static size_t cache_cap  = 0;      /* allocated capacity       */
static idx_t  cache_len  = 0;      /* stored number of squares */

/* Next integer to examine. */
static idx_t last_checked = 1;

/* Return the first decimal digit of a number.  */
static unsigned
first_decimal_digit(seq_t x)
{
    int d = digit_len(x);                  /* number of decimal digits */
    seq_t pow = pow10((idx_t)d - 1);        /* 10^(d-1)                 */
    seq_t leading = x / pow;                /* leading digit(s)         */
    seq_t first   = leading % 10;           /* extract first digit      */
    return (unsigned)first;                /* assume 0 <= first < 10   */
}

/* Grow the cache so that it contains at least n terms. */
static void
ensure_cache(idx_t n)
{
    if (cache_len >= n)
        return;

    /* Grow the array if necessary. */
    if (cache_cap < cache_len + 1) {
        size_t new_cap = (cache_cap == 0 ? 16 : cache_cap << 1);
        if (new_cap < cache_len + 1)
            new_cap = cache_len + 1;
        seq_t *tmp = (seq_t *)realloc(cache_sq, new_cap * sizeof(seq_t));
        cache_sq = tmp;
        cache_cap = new_cap;
    }

    /* Scan successive integers, storing those whose squares start with 4. */
    while (cache_len < n) {
        /* Check if the square would overflow for 64‑bit seq_t. */
        if (sizeof(seq_t) <= sizeof(unsigned long long)) {
            if (last_checked > 0 && last_checked > ULLONG_MAX / last_checked)
                break;                    /* cannot square further          */
        }

        seq_t sq = last_checked * last_checked;
        if (first_decimal_digit(sq) == 4) {
            cache_sq[cache_len] = sq;
            ++cache_len;
        }
        ++last_checked;
    }
}

/* ------------------------- public interface ------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                 /* undefined term outside offset */

    ensure_cache(n);
    if (n <= cache_len)
        return cache_sq[n - 1];

    /* If we reached the end of representable numbers, return 0. */
    return (seq_t)0;
}
