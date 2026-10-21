/*
 * A095437  a(n) = 21 written in base 10 - n.
 * https://oeis.org/A095437
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = decimal integer formed by the base‑(10‑n) representation
 *            of 21 (for n = 0 … 9). For n ≥ 10 the base is undefined; the
 *            routine falls back to the value 21
 * Range:     u8 n <= 7, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute base = 10 – n.  n is unsigned; cast to signed to avoid
       underflow when n > 10. */
    int64_t base = 10 - (int64_t)n;

    /* Handle the special bases explicitly.  Base 1 is unary (21 ones). */
    if (base == 1) {
        seq_t res = 0;
        for (int i = 0; i < 21; ++i) {
            res = res * 10 + 1;
        }
        return res;
    }

    /* For undefined or negative bases, return a default value (the first
       term of the sequence).  This matches the behaviour for 10‑n ≤ 0. */
    if (base <= 0) {
        return (seq_t)21;
    }

    /* Base ≥ 2.  21 is converted to the chosen base, then the digits are
       concatenated into a decimal integer.  Since 10‑n ≤ 10, each digit
       is a single decimal digit. */
    seq_t num = (seq_t)21;
    int digits[32];   /* more than enough for any base ≤ 10 */
    int sz = 0;

    seq_t b = (seq_t)base;
    while (num > 0) {
        int d = (int)(num % b);
        num = num / b;
        digits[sz++] = d;
    }

    seq_t res = 0;
    for (int i = sz - 1; i >= 0; --i) {
        res = res * 10 + (seq_t)digits[i];
    }
    return res;
}
