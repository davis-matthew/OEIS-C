/*
 * A021017  Decimal expansion of 1/13.
 * https://oeis.org/A021017
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A021017: digits of the decimal expansion of 1/13
   a(0)=0, a(1)=7, a(2)=6, a(3)=9, a(4)=2, a(5)=3, then repeats with period 6. */
seq_t a(idx_t n) {
    static const seq_t pat[6] = {0, 7, 6, 9, 2, 3};
    return pat[n % 6];
}
