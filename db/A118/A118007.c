/*
 * A118007  Triangle, diagonals generated from Lucas polynomials.
 * https://oeis.org/A118007
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) enumerates the triangle whose diagonals are the * Lucas
 *            polynomials P_d(x) defined by * * P_0(x) = 2 * P_1(x) = x + 2 *
 *            P_d(x) = (x + 2) * P_{d-1}(x) - P_{d-2}(x) (d >= 2) * * For row
 *            r and column k (0‑based), let d = r - k and x = k + 1. * Then
 *            the triangle entry is * * T(r,k) = P_d(x) * *...
 * Range:     u8 n <= 20, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t r = 0;
    while ((unsigned __int128)(r + 1) * (r + 2) / 2 <= n)
        ++r;

    /* Column index k within the row */
    idx_t offset = r * (r + 1) / 2;
    idx_t k = n - offset;
    idx_t d = r - k;                 /* distance from the rightmost diagonal */
    seq_t x = (seq_t)(k + 1);        /* evaluation point for the polynomial */

    /* Evaluate Lucas polynomial P_d at x using the recurrence */
    if (d == 0)
        return (seq_t)2;
    if (d == 1)
        return x + 2;

    seq_t prevprev = (seq_t)2;   /* P_0(x) */
    seq_t prev     = x + 2;      /* P_1(x) */
    for (idx_t i = 2; i <= d; ++i) {
        seq_t cur = ((x + 2) * prev) - prevprev;
        prevprev = prev;
        prev = cur;
    }
    return prev;                 /* P_d(x) */
}
