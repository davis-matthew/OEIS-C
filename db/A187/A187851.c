/*
 * A187851  Number of 3-step king-knight's tours (piece capable of both kinds
 *          of moves) on an n X n board summed over all starting positions.
 * https://oeis.org/A187851
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 240*n*n - 904*n + 832 for n > 3 * Initial values: a(1) =
 *            0, a(2) = 24, a(3) = 304 * (For n = 0 or below the offset, the
 *            value is defined as 0.)
 * Range:     u8 n <= 2, u16 n <= 18, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Handle the few base cases that are not covered by the quadratic formula */
    if (n == 0)
        return 0;
    if (n == 1)
        return 0;
    if (n == 2)
        return 24;
    if (n == 3)
        return 304;

    /* For n > 3 use the closed‑form expression.
       Use idx_t for intermediate arithmetic to avoid overflow on small seq_t types. */
    idx_t t = (idx_t)n;
    idx_t val = 240 * t * t - 904 * t + 832;
    return (seq_t)val;
}
