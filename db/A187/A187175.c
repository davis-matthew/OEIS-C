/*
 * A187175  Number of 5-step left-handed knight's tours (moves only out two,
 *          left one) on an n X n board summed over all starting positions.
 * https://oeis.org/A187175
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 100*n^2 - 920*n + 1984 (for n > 7); Special case values
 *            for n = 1..7 are hard‑coded. a(1)=0, a(2)=0, a(3)=0, a(4)=0,
 *            a(5)=16, a(6)=136, a(7)=456 (these are the first terms of the
 *            sequence)
 * Range:     u8 n <= 6, u16 n <= 30, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Handle the first seven terms explicitly */
    if (n <= 7) {
        static const seq_t small[] = {0, 0, 0, 0, 16, 136, 456};
        return small[n - 1];
    }
    /* For n > 7 use the empirical quadratic formula */
    seq_t N = (seq_t)n;
    return (N * N * 100) - (N * 920) + 1984;
}
