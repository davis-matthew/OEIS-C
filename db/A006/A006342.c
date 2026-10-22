/*
 * A006342  Coloring a circuit with 4 colors.
 * https://oeis.org/A006342
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1; a(n) = 3*a(n-1) + 1 for even n > 0, a(n) = 3*a(n-1) -
 *            2 for odd n (equivalent to a(n) = (3^(n+1)+5)/8 for n even,
 *            (3^(n+1)-1)/8 for n odd)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 41, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t v = 1;
    idx_t i;
    for (i = 1; i <= n; i++) {
        if (i % 2 == 0)
            v = v * 3 + 1;
        else
            v = v * 3 - 2;
    }
    return v;
}
