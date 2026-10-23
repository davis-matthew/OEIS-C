/*
 * A183730  1/16 the number of (n+1) X 2 0..7 arrays with every 2 X 2 subblock
 *          strictly increasing clockwise or counterclockwise with one
 *          decrease, and at least two adjacent blocks having the same
 *          increasing direction.
 * https://oeis.org/A183730
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical recurrence:
   a(n) = 18*a(n-1) - 46*a(n-2) - 342*a(n-3) - 33*a(n-4) + 435*a(n-5)
          - 157*a(n-6) - 37*a(n-7) + 21*a(n-8) - 2*a(n-9)
   initial values:
   a(1)=0, a(2)=21, a(3)=563, a(4)=9403, a(5)=135578,
   a(6)=1814815, a(7)=23204876, a(8)=287769879, a(9)=3491293862
*/

#include <stddef.h>
#include <stdint.h>

static const seq_t init_vals[10] = {
    0ULL,          /* dummy index 0 */
    0ULL,          /* a(1) */
    21ULL,         /* a(2) */
    563ULL,        /* a(3) */
    9403ULL,       /* a(4) */
    135578ULL,     /* a(5) */
    1814815ULL,    /* a(6) */
    23204876ULL,   /* a(7) */
    287769879ULL,  /* a(8) */
    3491293862ULL  /* a(9) */
};

#define MAX_N 100000                      /* safe upper bound for requested n */
static seq_t cache[MAX_N + 1];            /* 1‑indexed cache */
static idx_t cached_max = 0;              /* largest n already computed */

/* Main function required by the exercise */
seq_t a(idx_t n)
{
    if (n == 0) return 0ULL;             /* undefined, but harmless */

    if (cached_max < 9) {                /* initialize base values once */
        for (idx_t i = 1; i <= 9; ++i)
            cache[i] = init_vals[i];
        cached_max = 9;
    }

    if (n <= cached_max)                 /* value already known */
        return cache[n];

    /* Compute successive terms up to n */
    for (idx_t i = cached_max + 1; i <= n; ++i) {
        cache[i] = 18*cache[i-1]
                 - 46*cache[i-2]
                 - 342*cache[i-3]
                 - 33*cache[i-4]
                 + 435*cache[i-5]
                 - 157*cache[i-6]
                 - 37*cache[i-7]
                 + 21*cache[i-8]
                 - 2*cache[i-9];
    }
    cached_max = n;
    return cache[n];
}
