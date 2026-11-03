/*
 * A073053  Apply DENEAT operator (or the Sisyphus function) to n.
 * https://oeis.org/A073053
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = DENEAT(n) – concatenate (#even digits, #odd digits, total
 *            digits) of n (leading zeros omitted)
 * Range:     u8 n <= 199, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 70 terms through n = 9940 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static int digit_len_idx(idx_t x)
{
    return (x < 10) ? 1 : 2;          /* x is 0..20 for our use */
}

seq_t a(idx_t n)
{
    idx_t e = 0, o = 0, t = 0, m = n;

    if (m == 0) {                     /* 0 is an even digit, one digit total */
        e = 1;
        t = 1;
    } else {
        while (m > 0) {
            uint8_t d = m % 10;
            if ((d & 1) == 0)   e++;   /* even */
            else                o++;   /* odd */
            t++; m /= 10;
        }
    }

    int len_o = digit_len_idx(o);
    int len_t = digit_len_idx(t);

    /* multiplier for the leading even-count part: 10^(len_o + len_t) */
    seq_t mult_e = 1;
    for (int i = 0; i < len_o + len_t; ++i) mult_e *= 10;

    /* multiplier for the odd-count part: 10^(len_t) */
    seq_t mult_o = 1;
    for (int i = 0; i < len_t; ++i) mult_o *= 10;

    seq_t result = (seq_t)e * mult_e + (seq_t)o * mult_o + (seq_t)t;
    return result;
}
