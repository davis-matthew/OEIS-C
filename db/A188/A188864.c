/*
 * A188864  Number of n X 7 binary arrays without the pattern 0 1 diagonally,
 *          vertically or antidiagonally.
 * https://oeis.org/A188864
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 729*n - 987 for n > 4; a(1)=128, a(2)=577, a(3)=1220,
 *            a(4)=1931
 * Range:     u8 n <= 1, u16 n <= 91, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 4) {
        static const seq_t first[4] = {128, 577, 1220, 1931};
        return first[n-1];
    }
    seq_t res = (seq_t)n * 729;
    res -= 987;
    return res;
}
