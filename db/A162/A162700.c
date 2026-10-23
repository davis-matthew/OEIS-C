/*
 * A162700  Numbers k which are concatenations k = x//y such that y^2 - x^2 =
 *          k.
 * https://oeis.org/A162700
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth integer k that can be written as a concatenation
 *            * k = x//y with y having d decimal digits, where * y^2 - x^2 =
 *            k. * The function searches increasing y, tests for the algebraic
 *            conditions, * and caches found terms
 * Range:     u8 none, u16 n <= 1, u32 n <= 18, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   floor_sqrt, grow_seq, pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/grow_seq.h"
#include "../utils/pow_int.h"       /* pow_int(seq_t base, idx_t exp)   */
#include "../utils/floor_sqrt.h"    /* floor_sqrt(seq_t x)             */

static seq_t *cache     = NULL;  /* array of terms */
static size_t cache_cap = 0;     /* allocated capacity */
static size_t cache_len = 0;     /* number of terms stored */

static uint64_t digit_count(uint64_t n) /* number of decimal digits */
{
    uint64_t c = 0;
    do {
        ++c;
        n /= 10;
    } while (n);
    return c;
}

/* Ensure the cache can hold at least `need` elements. */
static void ensure_capacity(size_t need)
{
    if (need <= cache_cap) return;
    grow_seq(&cache, &cache_cap, need);
}

seq_t a(idx_t n)
{
    /* n == 0 is outside the defined offset; return 0. */
    if (n == 0) return 0;
    /* Initialise cache lazily. */
    if (cache == NULL) grow_seq(&cache, &cache_cap, 0);

    /* Persist the largest y processed across calls. */
    static idx_t last_y = 2;                /* start from y = 3 */

    while (cache_len < n) {
        ++last_y;
        idx_t  y = last_y;
        uint64_t d = digit_count(y);

        /* pow10d = 10^d as seq_t */
        seq_t pow10d = pow_int((seq_t)10, (idx_t)d);
        /* pow10_2d = (10^d)^2 */
        seq_t pow10_2d = pow10d * pow10d;

        /* t = y*(y-1) as seq_t */
        seq_t t = (seq_t)y * (seq_t)(y - 1);

        /* D = 10^(2d) + 4*t */
        seq_t D = pow10_2d + (seq_t)4 * t;

        /* s = floor_sqrt(D) */
        seq_t s = floor_sqrt(D);

        /* Check if s^2 == D */
        if (s * s == D) {
            seq_t diff = s - pow10d;
            /* diff must be even */
            if ((diff & (seq_t)1) == (seq_t)0) {
                seq_t x = diff >> 1;           /* x = diff / 2 */
                if (x > (seq_t)0) {
                    /* k = x*10^d + y */
                    seq_t k = x * pow10d + (seq_t)y;

                    /* Store term */
                    ensure_capacity(cache_len + 1);
                    cache[cache_len++] = k;
                    continue;  /* proceed to next y */
                }
            }
        }
        /* No valid term for this y; continue searching. */
    }
    return cache[n - 1];
}
