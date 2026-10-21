/*
 * A085085  Numerator of Sum_{i=2..t} (d(i)/d(i-1)-1), where d(1), ..., d(t)
 *          are the divisors of n.
 * https://oeis.org/A085085
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of Sum_{i=2..t} (d(i)/d(i‑1)-1), where
 *            d(1)…d(t) are the divisors of n sorted increasingly
 * Range:     u8 n <= 33, u16 n <= 2430, u32 n <= 9741, u64 n <= 9741,
 *            u128 n <= 9741, big any n
 * Helpers:   get_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation enumerates the divisors of n, builds successive
   fractions, and reduces them at each step to keep the intermediate
   values as small as possible.  No external dependencies are required. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/get_divisors.h"

/* ------------------------------------------------------------------ */
/* Euclidean gcd for seq_t                                            */
static seq_t gcd_seq(seq_t a, seq_t b)
{
    while (b) {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

/* ------------------------------------------------------------------ */

/* ------------------------------------------------------------------ */
/* Public sequence routine                                            */
seq_t a(idx_t n)
{
    if (n == 0)            /* undefined below offset – return 0 */
        return 0;

    /* buffer big enough for all divisors of n (≤256 for n≤10⁴); safe also
       for larger n, but memory usage stays small. */
    idx_t divs[256];
    size_t cnt;
    get_divisors(n, divs, &cnt);
    if (cnt <= 1)          /* n = 1  → 0 */
        return 0;

    seq_t sum_num = 0;     /* current numerator   */
    seq_t sum_den = 1;     /* current denominator */

    for (size_t i = 1; i < cnt; ++i) {
        seq_t diff = (seq_t)(divs[i] - divs[i - 1]);   /* d_i - d_{i-1} */
        seq_t denom = (seq_t)divs[i - 1];              /* d_{i-1}     */

        /* add fraction diff/denom to sum_num/sum_den
           while reducing the size of intermediate products. */

        seq_t g = gcd_seq(sum_den, denom);
        seq_t mul1 = denom / g;           /* d_{i-1}/g      */
        seq_t mul2 = sum_den / g;         /* sum_den/g      */

        seq_t new_num = sum_num * mul1 + diff * mul2;
        seq_t new_den = sum_den * mul1;   /* lcm of denominators */

        seq_t g2 = gcd_seq(new_num, new_den);
        sum_num = new_num / g2;
        sum_den = new_den / g2;
    }

    return sum_num;       /* numerator after reduction */
}
