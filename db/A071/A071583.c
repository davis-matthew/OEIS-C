/*
 * A071583  Powers of 5 written backwards.
 * https://oeis.org/A071583
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = reverse_decimal(5^n)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 12, u64 n <= 27, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute 5^n then reverse its decimal digits. */
seq_t a(idx_t n)
{
    seq_t pow = 1;
    for (idx_t i = 0; i < n; ++i) {
        pow *= 5;
    }
    seq_t rev = 0;
    seq_t q = pow;
    while (q > 0) {
        seq_t digit = q % 10;
        rev = rev * 10 + digit;
        q = q / 10;
    }
    return rev;
}
