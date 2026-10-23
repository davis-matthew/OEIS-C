/*
 * A026476  For n>3, a(n) = 7*n - 21 + 2*(-1)^n.
 * https://oeis.org/A026476
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   for n>3, a(n) = 7*n - 21 + 2*(-1)^n; a(1)=1, a(2)=3, a(3)=4
 * Range:     u8 n <= 39, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)3;
    if (n == 3) return (seq_t)4;
    seq_t nn = (seq_t)n;
    seq_t val = nn * (seq_t)7 - (seq_t)21;
    if ((n & 1ULL) == 0) {
        val += (seq_t)2;
    } else {
        val -= (seq_t)2;
    }
    return val;
}
