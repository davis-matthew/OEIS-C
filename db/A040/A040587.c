/*
 * A040587  Continued fraction for sqrt(612).
 * https://oeis.org/A040587
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less, mult
 * Formula:   a(0)=24; for n>=1, a(n) follows the period 8 pattern
 *            1,2,1,4,1,2,1,48 cyclically starting at n=1. Implementation uses
 *            a lookup table and modulo 8
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) {
        return (seq_t)24;
    }
    static const seq_t table[8] = {
        (seq_t)48, (seq_t)1, (seq_t)2, (seq_t)1,
        (seq_t)4,  (seq_t)1, (seq_t)2, (seq_t)1
    };
    return table[n % 8];
}
