/*
 * A093255  a(n) is the largest number such that all of a(n)'s length-n
 *          substrings are distinct and divisible by 55.
 * https://oeis.org/A093255
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) equals the largest integer whose all length‑n substrings
 *            are distinct and divisible by 55. It is given by the following
 *            closed forms: • n = 1 : 0 • n = 2 : 55 • n odd (n = 2k+1) : 6^k
 *            0 5^k 0 (k = n‑1) • n even (n ≥ 4, n = 2k) : 5^(n‑2) 00 5^(n‑2)
 *            0 Where 5^m denotes the decimal number consi...
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 4, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow10.h"   /* pow10(idx_t) returns 10^exp as seq_t */

/* Return the integer consisting of `len` copies of decimal digit `d`
   (0 ≤ d ≤ 9).  len may be 0 (returns 0). */
static seq_t repunit_digit(idx_t len, int d)
{
    if (len == 0) return (seq_t)0;
    seq_t p = pow10(len);          /* 10^len */
    seq_t v = p - (seq_t)1;        /* 10^len - 1 */
    v /= (seq_t)9;                 /* div by 9: (10^len-1)/9 = 111…1 */
    return v * (seq_t)d;           /* d * 111…1 = d repeated len times */
}

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)55;

    /* Odd n: a(n) = 6^(n‑1) 0 5^(n‑1) 0 */
    if (n & 1) {
        idx_t k = n - 1;                /* number of 6’s and 5’s */
        seq_t sixes = repunit_digit(k, 6);  /* 6 repeated k times */
        seq_t fives = repunit_digit(k, 5);  /* 5 repeated k times */

        seq_t part1 = sixes * pow10(k + 2); /* shift by k+2 digits */
        seq_t part2 = fives * (seq_t)10;     /* shift for final 0 */

        return part1 + part2;               /* end digit 0 is implicit */
    }

    /* Even n (n ≥ 4): a(n) = 5^(n‑2) 00 5^(n‑2) 0 */
    idx_t m = n - 2;                 /* number of 5’s in each block */
    seq_t fives = repunit_digit(m, 5);  /* 5 repeated m times */

    seq_t part1 = fives * pow10(m + 3); /* shift by m+3 digits */
    seq_t part2 = fives * (seq_t)10;    /* shift for final 0 */

    return part1 + part2;
}
