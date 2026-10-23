/*
 * A037770  Base 9 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 3,0,2,1.
 * https://oeis.org/A037770
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(1)=3; for n>1, a(n)=9*a(n-1)+d(n),
 * where d(n) is the periodic digit sequence 3,0,2,1 (period 4).
 */

seq_t a(idx_t n) {
    static const int digits[4] = {3, 0, 2, 1};
    seq_t value = 0;
    for (idx_t i = 1; i <= n; ++i) {
        int d = digits[(int)((i - 1) & 3u)];
        value = value * (seq_t)9 + (seq_t)d;
    }
    return value;
}
