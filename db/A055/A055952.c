/*
 * A055952  n + reversal of base 6 digits of n (written in base 10).
 * https://oeis.org/A055952
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = n + reverse_base6(n) (reverse the base‑6 digits of n,
 *            interpret as base‑10, and add n)
 * Range:     u8 n <= 58, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t rev = 0;
    idx_t temp = n;
    while (temp) {
        idx_t digit = temp % 6;
        rev = rev * ((seq_t)6) + ((seq_t)digit);
        temp /= 6;
    }
    return ((seq_t)n) + rev;
}
