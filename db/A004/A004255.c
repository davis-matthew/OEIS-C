/*
 * A004255  a(n) = n*(n + 1)*(n^2 - 3*n + 6)/8.
 * https://oeis.org/A004255
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(n^2 - 3*n + 6)/8 = (n*(n+1)/2) * ((n-1)*(n-2)/2
 *            + 2) / 2
 * Range:     u8 n <= 7, u16 n <= 27, u32 n <= 431, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t A, B, C;
    /* A = n*(n+1)/2, dividing the even factor first */
    if (n % 2 == 0)
        A = (seq_t)(n / 2) * ((seq_t)n + 1);
    else
        A = (seq_t)n * (seq_t)((n + 1) / 2);
    /* C = (n-1)*(n-2)/2, dividing the even factor first */
    if ((n - 1) % 2 == 0)
        C = (seq_t)((n - 1) / 2) * (seq_t)(n - 2);
    else
        C = (seq_t)(n - 1) * (seq_t)((n - 2) / 2);
    B = C + 2;
    /* A*B is even; halve the even factor before multiplying */
    if (A % 2 == 0)
        return (A / 2) * B;
    return A * (B / 2);
}
