/*
 * A006428  Number of loopless tree-rooted planar maps with 3 vertices and n
 *          faces and no isthmuses.
 * https://oeis.org/A006428
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(n^3+6*n^2+11*n-42)/24 for n > 1, a(1) = 0
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 158, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t P, Q;
    idx_t pm, g, d;

    if (n < 2) return 0;

    /* P = n*(n+1)/2, halving first */
    if (n % 2 == 0)
        P = (seq_t)(n / 2) * ((seq_t)n + 1);
    else
        P = (seq_t)n * ((seq_t)((n + 1) / 2));

    /* Q = n^3+6n^2+11n-42 = (n+1)(n+2)(n+3) - 48, positive for n >= 2 */
    Q = ((seq_t)n + 1) * ((seq_t)n + 2) * ((seq_t)n + 3) - 48;

    /* a(n) = P*Q/12: divide 12 between P and Q (12/gcd(P,12) divides Q) */
    pm = (idx_t)((n % 24) * ((n + 1) % 24) / 2 % 12);
    g = 1;
    for (d = 12; d >= 1; d--) {
        if (12 % d == 0 && pm % d == 0) { g = d; break; }
    }
    P /= g;
    Q /= (12 / g);
    return P * Q;
}
