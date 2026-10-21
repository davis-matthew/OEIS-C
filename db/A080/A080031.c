/*
 * A080031  a(n) is taken to be the smallest positive integer not already
 *          present which is consistent with the condition "n is a member of
 *          the sequence if and only if a(n) is congruent to 2 mod 3".
 * https://oeis.org/A080031
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(3m)=3*m+1, a(3m+1)=6*m+2, a(3m+2)=6*m+5
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t m = n / 3;
    idx_t r = n % 3;
    switch (r) {
        case 0:
            return (seq_t)(3 * m + 1);
        case 1:
            return (seq_t)(6 * m + 2);
        case 2:
            return (seq_t)(6 * m + 5);
    }
    return 0; /* unreachable */
}
