/*
 * A039157  Numbers whose base-11 representation has the same number of 0's
 *          and 2's.
 * https://oeis.org/A039157
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   scan natural numbers, keep those where in base‑11 representation
 *            * the count of digit 0 equals the count of digit 2. a(n) is the
 *            n‑th such * number (OEIS A039157)
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Validated: partial, pass, 70 terms through n = 70 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>  /* for bool */

static bool zeros_twos_equal(seq_t x)
{
    unsigned zeros = 0, twos = 0;
    while (x > 0) {
        seq_t rem = x % 11;
        if (rem == 0) ++zeros;
        else if (rem == 2) ++twos;
        x /= 11;
    }
    return zeros == twos;
}

seq_t a(idx_t n)
{
    seq_t k = (seq_t)1;
    idx_t cnt = 0;
    for (;; ++k) {
        if (zeros_twos_equal(k)) {
            if (++cnt == n)
                return k;
        }
    }
}
