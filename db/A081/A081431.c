/*
 * A081431  RevBinary(RevDecimal(n)), where RevBinary(m) is the binary
 *          reversal of m (A030101) and RevDecimal(m) is the decimal reversal
 *          of m (A004086).
 * https://oeis.org/A081431
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = rev_bin( rev_dec(n) )
 * Range:     u8 n <= 102, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 90 terms through n = 972 (2026-11-01)
 * Helpers:   reverse_bits, reverse_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/reverse_bits.h"
#include "../utils/reverse_digits.h"
/*  rev_dec reverses the decimal digits of n (A004086).  */
/*  rev_bin reverses the bits of that result (A030101).  */

seq_t a(idx_t n) {
    seq_t x = (seq_t)n;
    seq_t rd = reverse_digits(x);
    return reverse_bits(rd);
}
