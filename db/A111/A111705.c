/*
 * A111705  a(n) = concatenation of n times each digit of n divided by n.
 * https://oeis.org/A111705
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = (concatenation of n*d for each decimal digit d of n) / n
 * Range:     u8 n <= 10, u16 n <= 33, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   digits, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"
#include "../utils/digits.h"

seq_t a(idx_t n)
{
    /* Preserve original n as a seq_t for final division */
    seq_t n_orig = (seq_t)n;

    /* Count digits of n (n >= 1) */
    idx_t len = 0;
    idx_t tmp_n = n;
    while (tmp_n > 0) {
        len++;
        tmp_n /= 10;
    }

    /* 10^(len-1) to extract the most significant digit first */
    idx_t mult = 1;
    for (idx_t i = 1; i < len; ++i)
        mult *= 10;

    seq_t concat = 0;
    tmp_n = n;            /* working copy of n */

    for (idx_t i = 0; i < len; ++i) {
        /* Current most‑significant decimal digit */
        idx_t d = tmp_n / mult;

        /* Remaining number for next iteration */
        tmp_n %= mult;
        if (mult > 1)
            mult /= 10;
        else
            mult = 0;   /* keep it 0 for safety after last digit */

        /* Product n*d */
        seq_t prod = n_orig * (seq_t)d;

        /* Length of decimal representation of prod (at least 1) */
        int lenp = 0;
        seq_t tmp = prod;
        while (tmp > 0) {
            lenp++;
            tmp /= 10;
        }
        if (lenp == 0)          /* prod == 0 */
            lenp = 1;

        /* 10^lenp as seq_t */
        seq_t p10 = seq_pow((seq_t)10, (idx_t)lenp);

        /* Append prod to the concatenated value */
        concat = concat * p10 + prod;
    }

    /* Final division by n */
    return concat / n_orig;
}
