/*
 * A133048  Powerback(n): reverse the decimal expansion of n, drop any leading
 *          zeros, then apply the powertrain map of A133500 to the resulting
 *          number.
 * https://oeis.org/A133048
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = power‑train of reverse(n) (decimal digits, leading zeros
 *            dropped)
 * Range:     u8 n <= 36, u16 n <= 66, u32 n <= 2498, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow_uint, rev
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/rev.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    idx_t rev_n   = rev(n);          /* reverse decimal expansion, drop leading zeros */
    uint8_t digits[20];
    int cnt = 0;

    if (rev_n == 0) {                /* handle the zero case */
        digits[0] = 0;
        cnt = 1;
    } else {
        /* collect digits least‑significant first */
        while (rev_n > 0) {
            digits[cnt++] = (uint8_t)(rev_n % 10);
            rev_n /= 10;
        }
    }

    seq_t result = 1;
    int i = cnt - 1;                 /* iterate from most‑significant digit */

    while (i >= 0) {
        uint8_t base = digits[i];
        int exp = 1;                 /* default exponent */
        if (i > 0) {                 /* a following digit supplies the exponent */
            exp = digits[i - 1];
            i--;                     /* skip the exponent digit */
        }
        result *= pow_uint((uint64_t)base, exp);
        i--;
    }

    return result;
}
