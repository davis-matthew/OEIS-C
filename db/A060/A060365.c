/*
 * A060365  Multiples of one thousand which are described by single words in
 *          American English.
 * https://oeis.org/A060365
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, fini, nonn, word
 * Formula:   a(n) = 10^(3*n)
 * Range:     u8 none, u16 n <= 1, u32 n <= 3, u64 n <= 6, u128 n <= 10,
 *            big any n
 * Validated: partial, pass, 10 terms through n = 10 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0)               /* a(0) is undefined; return 0 for safety */
        return (seq_t)0;
    return pow10(3 * n);
}
