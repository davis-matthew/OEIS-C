/*
 * A106371  Representation of n in base b, where b is minimal such that n
 *          contains no zeros: b = A106370(n).
 * https://oeis.org/A106371
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Computes the OEIS sequence A106371: representation of n in the
 * smallest base b (b ≥ 2) such that the base‑b expansion of n contains
 * no zero digit.  The result is returned as a decimal integer made
 * by concatenating the base‑b digits (each digit is 1–9 when the
 * minimal base is ≤10, which is the case for n ≤ 359).
 */
#include <stdbool.h>

seq_t a(idx_t n) {
    /* Find the minimal base b */
    idx_t b;
    for (b = 2; ; ++b) {
        idx_t x = n;
        bool ok = true;
        if (b > n) {          /* representation is a single digit n */
            break;
        }
        while (x > 0) {
            idx_t d = x % b;
            if (d == 0) {
                ok = false;
                break;
            }
            x /= b;
        }
        if (ok)
            break;
    }

    /* Convert the expansion to a decimal integer */
    idx_t x = n;
    seq_t res = 0;
    idx_t digits[64];
    idx_t len = 0;

    if (b > n) {
        digits[0] = n;
        len = 1;
    } else {
        while (x > 0) {
            digits[len++] = x % b;
            x /= b;
        }
    }

    /* Append digits from most significant to least significant */
    for (idx_t i = 0; i < len; ++i) {
        idx_t d = digits[len - 1 - i];

        if (d < 10) {
            res = res * 10 + (seq_t)d;
        } else {
            /* Append each decimal digit of d */
            idx_t tmp = d;
            idx_t part[32];
            idx_t part_len = 0;
            while (tmp > 0) {
                part[part_len++] = tmp % 10;
                tmp /= 10;
            }
            for (idx_t j = 0; j < part_len; ++j) {
                res = res * 10 + (seq_t)part[part_len - 1 - j];
            }
        }
    }

    return res;
}
