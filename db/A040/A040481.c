/*
 * A040481  Continued fraction for sqrt(504).
 * https://oeis.org/A040481
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   a(n) = 22 if n=0, otherwise a(n) = 2 if n%4==1 or 3, 4 if
 *            n%4==2, and 44 if n%4==0
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)22;
    idx_t r = n % 4;
    if (r == 1 || r == 3)
        return (seq_t)2;
    if (r == 2)
        return (seq_t)4;
    return (seq_t)44;
}
