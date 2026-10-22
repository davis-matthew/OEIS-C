/*
 * A005779  a(n) = largest integer such that every tournament on n nodes
 *          contains a consistent set of n arcs.
 * https://oeis.org/A005779
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n^2/4) for n ≤ 7; floor((n^2 + 2n)/4) for 8 ≤ n <
 *            12; floor((n^2 + 2n + 8)/4) for n ≥ 12
 * Range:     u8 n <= 13, u16 n <= 13, u32 n <= 13, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    
    if (n <= 7) {
        return nn * nn / 4;
    } else if (n < 12) {
        return (nn * nn + 2 * nn) / 4;
    } else {
        return (nn * nn + 2 * nn + 8) / 4;
    }
}
