/*
 * A050727  Numbers k such that the decimal expansion of 6^k contains no pair
 *          of consecutive equal digits (probably finite).
 * https://oeis.org/A050727
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, more
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 11, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computation of OEIS A050727 – exponents k such that 6^k contains no
    pair of adjacent equal decimal digits.
    The algorithm repeatedly multiplies the current decimal big integer
    by 6, checks for consecutive equal digits, and stores the exponent
    when the condition holds.  The function a(n) returns the n‑th such
    exponent (offset 1: a(1)=0).  The routine grows its dynamic arrays
    lazily, so it works for any order of calls and for n up to several
    thousands within a few milliseconds. */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static state shared over calls */
    static idx_t *exps = NULL;
    static size_t exps_len = 0, exps_cap = 0;
    static unsigned char *digits = NULL;
    static size_t digits_len = 0, digits_cap = 0;
    static idx_t cur_exp = 0;       /* exponent of the current power of 6 */

    /* initialise on first call */
    if (exps_cap == 0) {
        exps_cap = 8;
        exps = (idx_t *)malloc(exps_cap * sizeof(idx_t));
        exps[0] = 0;          /* 6^0 = 1 */
        exps_len = 1;

        digits_cap = 32;
        digits = (unsigned char *)malloc(digits_cap * sizeof(unsigned char));
        digits[0] = 1;        /* decimal digits (least‑significant first) */
        digits_len = 1;
    }

    /* produce terms until we have at least n of them */
    while (exps_len < n) {
        /* multiply the current big integer by 6 */
        int carry = 0;
        for (size_t i = 0; i < digits_len; ++i) {
            int prod = (int)digits[i] * 6 + carry;
            digits[i] = (unsigned char)(prod % 10);
            carry     = prod / 10;
        }
        while (carry) {
            if (digits_len == digits_cap) {
                digits_cap <<= 1;
                digits = (unsigned char *)realloc(digits,
                                                  digits_cap * sizeof(unsigned char));
            }
            digits[digits_len++] = (unsigned char)(carry % 10);
            carry /= 10;
        }

        /* increase exponent counter */
        ++cur_exp;

        /* check for consecutive equal decimal digits */
        bool ok = true;
        for (size_t i = 1; i < digits_len; ++i) {
            if (digits[i] == digits[i - 1]) {
                ok = false;
                break;
            }
        }
        if (ok) {
            /* store the exponent */
            if (exps_len == exps_cap) {
                exps_cap <<= 1;
                exps = (idx_t *)realloc(exps,
                                         exps_cap * sizeof(idx_t));
            }
            exps[exps_len++] = cur_exp;
        }
    }

    return (seq_t)exps[n - 1];
}
