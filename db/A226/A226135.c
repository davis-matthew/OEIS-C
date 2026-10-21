/*
 * A226135  Let abcd... be the decimal expansion of n. Number of iterations of
 *          the map n -> f(n) needed to reach a number < 10, where f(n) = a^b
 *          + c^d + ... which ends in an exponent or a base according as the
 *          number of digits is even or odd.
 * https://oeis.org/A226135
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of iterations of the map * n → f(n) needed to
 *            reach a number < 10, * where f(n) = a^b + c^d + ... with the
 *            digits of n paired * as base^exponent (even number of digits)
 *            or, if the * number of digits is odd, the last digit is taken as
 *            base^1
 * Range:     u8 n <= 25, u16 n <= 25, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow, get_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/big_pow.h"
#include "../utils/get_digits.h"

static seq_t f_transform(seq_t n)
{
    /* Extract decimal digits of n, least‑significant first.
       Array size 32 is large enough for any 64‑bit integer. */
    uint8_t digits[32];
    size_t len;
    get_digits(n, digits, &len);

    seq_t sum = 0;
    int i = (int)len - 1;   /* index of most significant digit */

    if ((len & 1) == 0) {   /* even number of digits: pair (base,exp) */
        for (; i > 0; i -= 2) {
            seq_t base = (seq_t)digits[i];
            seq_t exp  = (seq_t)digits[i - 1];
            seq_t term = (base == 0 && exp == 0) ? (seq_t)1 : big_pow(base, exp);
            sum += term;
        }
    } else {                /* odd number: last digit is base^1 */
        for (; i > 1; i -= 2) {
            seq_t base = (seq_t)digits[i];
            seq_t exp  = (seq_t)digits[i - 1];
            seq_t term = (base == 0 && exp == 0) ? (seq_t)1 : big_pow(base, exp);
            sum += term;
        }
        /* last single digit as base^1 */
        seq_t term = (seq_t)digits[0];
        sum += term;
    }

    return sum;
}

seq_t a(idx_t n)
{
    seq_t val = (seq_t)n;
    if (val < (seq_t)10)
        return 0;

    seq_t steps = 0;
    while (val >= (seq_t)10) {
        val = f_transform(val);
        ++steps;
    }
    return steps;
}
