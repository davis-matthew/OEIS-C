/*
 * A083453  a(n) = (concatenation of numbers from n to 1) - n^n.
 * https://oeis.org/A083453
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, sign
 * Formula:   a(n) = (concatenation of numbers from n to 1) - n^n
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 10, i64 n <= 14, i128 n <= 23,
 *            big any n
 * Helpers:   big_pow, digit_count, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"
#include "../utils/big_pow.h"
#include "../utils/digit_count.h"

seq_t a(idx_t n)
{
    seq_t concat = 0;
    for (idx_t k = n; k; --k) {
        uint64_t d = digit_count(k);          /* number of decimal digits in k */
        seq_t mult = pow10((idx_t)d);         /* 10^d as seq_t */
        concat = concat * mult + (seq_t)k;    /* append k to the right */
    }
    seq_t power = big_pow((seq_t)n, (seq_t)n); /* n^n */
    return concat - power;
}
