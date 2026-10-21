/*
 * A061309  a(n) is the smallest number required to make the concatenation of
 *          a(n) and 2n+1 a prime, or -1 if no such number exists.
 * https://oeis.org/A061309
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, base
 * Formula:   a(n) = smallest a such that concat(a, 2n+1) is prime, -1
 *            otherwise
 * Range:     i8 n <= 9, i16 n <= 91, i32 n <= 91, i64 n <= 91, i128 n <= 91,
 *            big any n
 * Helpers:   digit_len, is_prime_big, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/digit_len.h"
#include "../utils/pow10.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    seq_t t = (seq_t)n * 2 + 1;           // 2n+1
    /* If t ends in 5 and t > 5, no concatenation can be prime */
    if (t % 5 == 0 && t != (seq_t)5)
        return -1;

    int d = digit_len(t);                 // number of decimal digits of t
    seq_t pow_t = pow10((idx_t)d);        // 10^d

    seq_t a_val = 0;
    while (1) {
        seq_t candidate = a_val * pow_t + t;
        if (is_prime_big(candidate))
            return a_val;
        ++a_val;
    }
}
