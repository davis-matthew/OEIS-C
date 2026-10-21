/*
 * A234356  Decimal expansion of B(18) = 43867/798, the 18th Bernoulli number.
 * https://oeis.org/A234356
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = the n-th decimal digit (including the integer part) of
 *            43867/798. Offset 2, so a(2) is the first digit
 * Range:     u8 none, u16 n <= 101, u32 n <= 101, u64 n <= 101, u128 n <= 101,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 2) return 0;                     /* undefined for n<2 */

    /* fraction 43867/798 */
    const idx_t NUM = 43867ULL;
    const idx_t DEN = 798ULL;

    seq_t num = (seq_t)NUM;
    seq_t den = (seq_t)DEN;

    /* integer part of the fraction */
    seq_t int_part = num / den;

    /* count digits of the integer part */
    idx_t int_digits = 0;
    seq_t tmp = int_part;
    if (tmp == 0) {
        int_digits = 1;
    } else {
        while (tmp > 0) {
            tmp /= 10;
            int_digits++;
        }
    }

    /* 0‑based position in the overall digit sequence */
    idx_t pos = n - 2;

    if (pos < int_digits) {                 /* digit from the integer part */
        idx_t power = int_digits - 1 - pos;
        seq_t div = 1;
        for (idx_t i = 0; i < power; ++i)
            div *= 10;
        seq_t digit = (int_part / div) % 10;
        return digit;
    }

    /* digit from the fractional part */
    idx_t k = pos - int_digits;            /* 0‑based index among fractional digits */
    seq_t r = num % den;                   /* initial remainder */

    seq_t digit = 0;
    for (idx_t i = 0; i <= k; ++i) {       /* compute the (k+1)‑th decimal digit */
        r *= 10;
        digit = r / den;
        r %= den;
    }
    return digit;
}
