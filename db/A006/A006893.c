/*
 * A006893  Smallest number whose representation requires n triangular numbers
 *          with greedy algorithm; also number of 1-2 rooted trees of height
 *          n.
 * https://oeis.org/A006893
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n+1) = a(n)*(a(n)+3)/2, a(1) = 1
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 7, u64 n <= 8, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t v = 1;
    for (idx_t i = 1; i < n; i++) {
        /* halve the even factor first so the intermediate product does not overflow */
        if (v % 2 == 0)
            v = (v / 2) * (v + 3);
        else
            v = v * ((v + 3) / 2);
    }
    return v;
}
