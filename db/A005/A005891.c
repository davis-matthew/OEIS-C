/*
 * A005891  Centered pentagonal numbers: (5n^2+5n+2)/2; crystal ball sequence
 *          for 3.3.3.4.4. planar net.
 * https://oeis.org/A005891
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*T(n) + 1, where T(n) = n*(n+1)/2
 * Range:     u8 n <= 9, u16 n <= 161, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t x = (seq_t)n;
    seq_t triangular;
    if (x & 1) {
        triangular = x * ((x + 1) / 2);
    } else {
        triangular = (x / 2) * (x + 1);
    }
    return 5 * triangular + 1;
}
