/*
 * A028431  Clog sequence in base 10. Right to left concatenation of n,
 *          int(log_10(n)), int(log_10(int(log_10(n)))),... in base10.
 * https://oeis.org/A028431
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = concat(n, floor(log10(n)),
 *            floor(log10(floor(log10(n)))),…, base10 (right to left)
 * Range:     u8 n <= 55, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   digit_len, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes the Clog sequence: right‑to‑left decimal concatenation of n, its floor base‑10 log,
    its floor base‑10 log again, etc. */

#include <stdint.h>
#include "../utils/digit_len.h"
#include "../utils/pow10.h"

extern seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    seq_t result = (seq_t)n;
    idx_t digits = (idx_t)digit_len(result);   /* number of decimal digits of n */
    seq_t t       = (seq_t)(digits - 1);       /* floor(log10(n)) */

    while (t > 0) {
        /* shift current result left by number of its digits and prepend t */
        idx_t res_digits = (idx_t)digit_len(result);   /* digits of current result */
        seq_t pow = pow10(res_digits);
        result = t * pow + result;

        /* next floor log */
        idx_t t_digits = (idx_t)digit_len(t);
        t = (seq_t)(t_digits - 1);
    }

    return result;
}
