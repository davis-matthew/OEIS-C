/*
 * A060406  Duplicate of A001839.
 * https://oeis.org/A060406
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = n*(n-2)/6 for n≡0,2 (mod 6); a(n) = n*(n-1)/6 for n≡1,3
 *            (mod 6); a(n) = (n^2-2*n-2)/6 for n≡4 (mod 6); a(n) =
 *            (n^2-n-2)/6 - 1 for n≡5 (mod 6) (same as A001839)
 * Range:     i8 n <= 26, i16 n <= 26, i32 n <= 26, i64 n <= 26, i128 n <= 26,
 *            big any n
 * Validated: partial, pass, 26 terms through n = 26 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t A001839_a(idx_t n)
{
    seq_t N = (seq_t)n;
    int r = (int)(n % 6);

    if (r == 0 || r == 2) {
        return N * (N - 2) / 6;
    } else if (r == 1 || r == 3) {
        return N * (N - 1) / 6;
    } else if (r == 4) {
        return (N * N - 2 * N - 2) / 6;
    } else { /* r == 5 */
        return (N * N - N - 2) / 6 - 1;
    }
}

seq_t a(idx_t n)
{
    return A001839_a(n);
}
