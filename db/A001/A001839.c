/*
 * A001839  The coding-theoretic function A(n,4,3).
 * https://oeis.org/A001839
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = n*(n-2)/6 for n≡0,2 (mod 6); a(n) = n*(n-1)/6 for n≡1,3
 *            (mod 6); a(n) = (n^2-2*n-2)/6 for n≡4 (mod 6); a(n) =
 *            (n^2-n-2)/6 - 1 for n≡5 (mod 6)
 * Range:     u8 n <= 40, u16 n <= 628, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    int r = n % 6;
    
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
