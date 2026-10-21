/*
 * A070064  Decimal expansion of the molar gas constant R according to the
 *          2019 SI system in units J mol^-1 K^-1.
 * https://oeis.org/A070064
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  cons, fini, full, nonn
 * Formula:   a(n) = digit at position n of the decimal expansion of R
 *            (8.31446261815324). For n > 15 the sequence is zero
 * Range:     u8 n <= 15, u16 n <= 15, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* offset 1: a(1) = 8 */
    if (n == 0) return 0;          /* below offset, undefined – return 0 */
    if (n > 15) return 0;          /* R terminates after 14 decimal places */

    /* 15‑digit integer that represents 8.31446261815324
       (8 plus 14 decimal digits) */
    const unsigned long long NUM = 831446261815324ULL;

    /* compute 10^(15-n) */
    unsigned long long pow10 = 1;
    idx_t exp = 15 - n;
    while (exp--) pow10 *= 10ULL;

    unsigned long long div = NUM / pow10;
    seq_t digit = (seq_t)(div % 10ULL);
    return digit;
}
