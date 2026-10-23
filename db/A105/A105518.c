/*
 * A105518  Number of times 8 is the leading digit of the first n+1 Fibonacci
 *          numbers in decimal representation.
 * https://oeis.org/A105518
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = #{k : 0 <= k <= n and the leading decimal digit of
 *            Fibonacci number F_k is 8 }
 * Range:     u8 n <= 4972, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdbool.h>

/* pre‑computed whether the first 26 Fibonacci numbers have a leading
   digit of 8 (index 0 through 25; F_0 = 0 is not counted) */
static const bool lead8_small[26] = {
    false, /* 0 */
    false, /* 1 */
    false, /* 2 */
    false, /* 3 */
    false, /* 4 */
    false, /* 5 */
    true,  /* 6  -> 8 */
    false, /* 7 */
    false, /* 8 */
    false, /* 9 */
    false, /*10 */
    true,  /*11 -> 8 */
    false, /*12 */
    false, /*13 */
    false, /*14 */
    false, /*15 */
    false, /*16 */
    false, /*17 */
    false, /*18 */
    false, /*19 */
    false, /*20 */
    false, /*21 */
    false, /*22 */
    false, /*23 */
    false, /*24 */
    false  /*25 */
};

/* constants for logarithmic leading‑digit calculation */
static const long double LOG10_PHI     = log10l((1.0L + sqrtl(5.0L)) / 2.0L); /* log10(φ) */
static const long double LOG10_SQRT5   = log10l(sqrtl(5.0L));                 /* log10(√5) */
static const long double LOG10_8       = log10l(8.0L);
static const long double LOG10_9       = log10l(9.0L);

/*------------------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    seq_t count = 0;

    for (idx_t k = 0; k <= n; ++k) {
        /* Fibonacci(0) = 0 has no leading digit other than 0 */
        if (k == 0)
            continue;

        if (k < 26) {                   /* small Fibonacci numbers – exact check */
            if (lead8_small[(int)k])
                count++;
        } else {                         /* use analytic logarithm for large k */
            long double x = (long double)k * LOG10_PHI - LOG10_SQRT5;
            long double frac = x - floorl(x);            /* fractional part [0,1) */
            if (frac >= LOG10_8 && frac < LOG10_9)
                count++;
        }
    }

    return count;
}
