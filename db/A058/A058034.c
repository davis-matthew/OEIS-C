/*
 * A058034  Number of numbers whose cube root rounds to n.
 * https://oeis.org/A058034
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3*n^2 + (n % 4 == 0 ? 1 : 0)
 * Range:     u8 n <= 9, u16 n <= 147, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    seq_t res = nn * nn * 3;
    if ((n & 3ULL) == 0) {        /* check n ≡ 0 (mod 4) */
        res += (seq_t)1;
    }
    return res;
}
