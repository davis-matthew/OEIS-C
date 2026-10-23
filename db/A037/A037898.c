/*
 * A037898  a(n) = (greatest base 4 digit of n) - (least base 4 digit of n).
 * https://oeis.org/A037898
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = (greatest base 4 digit of n) - (least base 4 digit of n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) {
        return (seq_t)0;
    }
    unsigned char min = 3;
    unsigned char max = 0;
    idx_t v = n;                     /* v is 64‑bit unsigned */
    /* Process base‑4 digits using bitwise operations. */
    while (v != 0) {
        unsigned char d = (unsigned char)(v & 3);  /* v % 4 */
        if (d < min) min = d;
        if (d > max) max = d;
        v >>= 2;                    /* v /= 4 */
    }
    return (seq_t)(max - min);
}
