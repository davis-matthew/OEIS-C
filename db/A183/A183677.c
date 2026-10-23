/*
 * A183677  Number of (n+1) X 5 0..7 arrays with every 2 X 2 subblock summing
 *          to 14.
 * https://oeis.org/A183677
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*8^n + 210*7^n + 1080*6^n + 2850*5^n + 5280*4^n +
 *            7650*3^n + 8760*2^n + 6930
 * Range:     u8 none, u16 none, u32 n <= 7, u64 n <= 19, u128 n <= 41,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Number of (n+1) × 5 arrays with entries 0..7 where each 2×2 subblock sums to 14 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) {          /* offset starts at 1 → a(1) is first term */
        return 0;
    }

    /* Compute each term  b^n  (b = 8,7,6,5,4,3,2) using arbitrary‑precision exponentiation */
    seq_t pow8 = big_pow((seq_t)8, (seq_t)n);
    seq_t pow7 = big_pow((seq_t)7, (seq_t)n);
    seq_t pow6 = big_pow((seq_t)6, (seq_t)n);
    seq_t pow5 = big_pow((seq_t)5, (seq_t)n);
    seq_t pow4 = big_pow((seq_t)4, (seq_t)n);
    seq_t pow3 = big_pow((seq_t)3, (seq_t)n);
    seq_t pow2 = big_pow((seq_t)2, (seq_t)n);

    /* Accumulate the linear combination */
    seq_t result = 0;
    result += (seq_t)8   * pow8;
    result += (seq_t)210 * pow7;
    result += (seq_t)1080 * pow6;
    result += (seq_t)2850 * pow5;
    result += (seq_t)5280 * pow4;
    result += (seq_t)7650 * pow3;
    result += (seq_t)8760 * pow2;
    result += (seq_t)6930;

    return result;
}
