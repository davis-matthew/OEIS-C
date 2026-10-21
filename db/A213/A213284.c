/*
 * A213284  Number of 5-length words w over n-ary alphabet such that for every
 *          prefix z of w we have #(z,a_i) = 0 or #(z,a_i) >= #(z,a_j) for all
 *          j>i and #(z,a_i) counts the occurrences of the i-th letter in z.
 * https://oeis.org/A213284
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(94-204*n+155*n^2-45*n^3+6*n^4)/6
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 85, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    unsigned __int128 N = n;
    unsigned __int128 n2 = N * N;
    unsigned __int128 n3 = n2 * N;
    unsigned __int128 n4 = n3 * N;

    unsigned __int128 first  = 6 * n4 + 155 * n2 + 94;
    unsigned __int128 second = 45 * n3 + 204 * N;

    unsigned __int128 term = first - second;          /* guaranteed non‑negative */
    unsigned __int128 num  = N * term;

    return (seq_t)(num / 6);
}
