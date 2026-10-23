/*
 * A029884  Second differences of Thue-Morse sequence A001285.
 * https://oeis.org/A029884
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = T(n) - 2*T(n+1) + T(n+2) * where T(k) is the Thue–Morse
 *            0/1 sequence (popcount(k) mod 2)
 * Range:     i8 n <= 80, i16 n <= 80, i32 n <= 80, i64 n <= 80, i128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t thue_t(idx_t x) {
    idx_t y = x;
    seq_t p = 0;
    while (y) {
        p ^= 1;
        y &= y - 1;   /* remove lowest set bit */
    }
    return p;
}
seq_t a(idx_t n) {
    seq_t t0 = thue_t(n);
    seq_t t1 = thue_t(n + 1);
    seq_t t2 = thue_t(n + 2);
    return t0 - 2 * t1 + t2;
}
