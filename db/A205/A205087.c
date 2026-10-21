/*
 * A205087  a(n) = n 7's sandwiched between two 1's.
 * https://oeis.org/A205087
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less, base
 * Formula:   a(0)=11, a(n)=10*a(n-1)+61
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (equivalently a(n) = (160*10^n - 61)/9) */

seq_t a(idx_t n)
{
    seq_t value = 11;
    while (n--) {
        value = value * (seq_t)10 + (seq_t)61;
    }
    return value;
}
