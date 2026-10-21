/*
 * A086020  a(n) = Sum_(i=1..n) binomial(i+2,3)^2 [ Sequential sums of the
 *          tetragonal numbers or "tetras" (pyramidal, square) raised to power
 *          2 (drawn from the 4th diagonal - left or right - of Pascal's
 *          Triangle) ].
 * https://oeis.org/A086020
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   a(n) = n*(n+1)*(n+2)*(n+3)*(2*n+3)*(5*n*n + 15*n + 1)/2520
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 50, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS A086020, where n >= 1.  */
/* Uses 128‑bit arithmetic to avoid overflow for n up to ~10⁴. */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;           /* defined only for n >= 1, but safe for n=0 */

    unsigned __int128 N = n;

    unsigned __int128 prod =
        N * (N + 1) *
        (N + 2) *
        (N + 3) *
        (2 * N + 3) *
        (5 * N * N + 15 * N + 1);

    prod /= 2520;

    return (seq_t)prod;
}
