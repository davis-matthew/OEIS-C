/*
 * A206392  Number of n X 4 0..5 arrays with no element equal to another
 *          within a city block distance of two, and new values 0..5
 *          introduced in row major order.
 * https://oeis.org/A206392
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A206392:
 * Number of n×4 0..5 arrays with no element equal to another within a city block distance of two,
 * new values 0..5 introduced in row major order.
 * Initial terms (n=1..15) are hard‑coded. For n>15 the recurrence relation
 *   a(n) = 14*a(n-1) -32*a(n-2) -281*a(n-3) +1738*a(n-4) -1728*a(n-5)
 *        -9767*a(n-6) +21963*a(n-7) -7410*a(n-8) +5706*a(n-9)
 *        -19704*a(n-10) +6028*a(n-11) +2512*a(n-12) -2176*a(n-13)
 * is used. The function caches computed values up to the largest n requested,
 * so calls can be in any order.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Initial values for n = 1 .. 15 */
static const seq_t init_vals[15] = {
    2ull,                     /* a(1) */
    25ull,                    /* a(2) */
    206ull,                   /* a(3) */
    1931ull,                  /* a(4) */
    16784ull,                 /* a(5) */
    151630ull,                /* a(6) */
    1343560ull,               /* a(7) */
    12046648ull,              /* a(8) */
    107401612ull,             /* a(9) */
    960610876ull,             /* a(10) */
    8576723918ull,            /* a(11) */
    76644197120ull,           /* a(12) */
    684569775162ull,          /* a(13) */
    6116039277778ull,         /* a(14) */
    54633888067900ull         /* a(15) */
};

/* Recurrence coefficients for n > 15 */
static const int rec_coeff[13] = {
    14,  -32,  -281, 1738,  -1728,  -9767, 21963,
    -7410,  5706, -19704, 6028, 2512, -2176
};

seq_t a(idx_t n)
{
    /* Handle the offset: a(1) is the first term */
    if (n == 0) return 0;

    /* Static cache for computed values */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static idx_t cache_cap = 0;

    /* Initialize cache with initial values */
    if (cache_len == 0) {
        cache_cap = 64;                      /* arbitrary initial capacity */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        for (idx_t i = 0; i < 15; ++i) cache[i] = init_vals[i];
        cache_len = 15;
    }

    /* Grow cache if needed */
    while (cache_cap < n) {
        idx_t new_cap = cache_cap * 2;
        if (new_cap < cache_cap) new_cap = n;   /* overflow guard */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* Memory allocation failed; return 0 as sentinel (unlikely) */
            return 0;
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing values */
    for (idx_t i = cache_len; i < n; ++i) {
        seq_t val;

        if (i < 15) {
            val = init_vals[i];
        } else {
            /* Compute using the recurrence.  Accumulate positive and negative parts
               separately to avoid signed intermediate overflows. */
            seq_t pos_sum = 0;
            seq_t neg_sum = 0;
            for (int k = 0; k < 13; ++k) {
                seq_t term = cache[i - 1 - k] * (seq_t)abs(rec_coeff[k]);
                if (rec_coeff[k] > 0)
                    pos_sum += term;
                else
                    neg_sum += term;
            }
            val = pos_sum - neg_sum;
        }

        cache[i] = val;
    }

    cache_len = n;
    return cache[n - 1];
}
