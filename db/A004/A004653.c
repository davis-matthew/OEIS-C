/*
 * A004653  Powers of 2 written in base 14. (Next term contains a non-decimal
 *          character.)
 * https://oeis.org/A004653
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 2^n written in base 14, with the base-14 digits read as a
 *            decimal number, i.e. a(n) = Sum_{k>=0} d_k * 10^k where 2^n =
 *            Sum_{k>=0} d_k * 14^k
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t p = 1;
    idx_t i;
    for (i = 0; i < n; i++)
        p *= 2;

    seq_t result = 0;
    seq_t multiplier = 1;
    while (p > 0) {
        seq_t digit = p % 14;
        result += digit * multiplier;
        multiplier *= 10;
        p /= 14;
    }
    return result;
}
