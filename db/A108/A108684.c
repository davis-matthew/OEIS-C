/*
 * A108684  a(n) = (n+1)*(n+2)*(n+3)*(19*n^3 + 111*n^2 + 200*n + 120)/720.
 * https://oeis.org/A108684
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)*(n+2)*(n+3)*(19*n^3 + 111*n^2 + 200*n + 120)/720
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    unsigned __int128 nn = n;
    unsigned __int128 part1 = (nn + 1) * (nn + 2) * (nn + 3);
    unsigned __int128 part2 = 19 * nn * nn * nn
                            + 111 * nn * nn
                            + 200 * nn
                            + 120;
    unsigned __int128 value = part1 * part2;
    value /= 720;
    return (seq_t)value;
}
