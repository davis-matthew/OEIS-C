/*
 * A215229  Number of length-6 0..k arrays connected end-around, with no
 *          sequence of L<n elements immediately followed by itself (periodic
 *          "squarefree").
 * https://oeis.org/A215229
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^6 - 6*n^4 + 5*n^3 + 5*n^2 - 5*n
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 40, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Compute in 128‑bit arithmetic to avoid overflow on 64‑bit builds. */
    unsigned __int128 N  = (unsigned __int128)n;
    unsigned __int128 n2 = N * N;          /* n^2   */
    unsigned __int128 n3 = n2 * N;         /* n^3   */
    unsigned __int128 n4 = n2 * n2;        /* n^4   */
    unsigned __int128 n6 = n3 * n3;        /* n^6   */

    unsigned __int128 res =
        n6
        - (unsigned __int128)6 * n4
        + (unsigned __int128)5 * n3
        + (unsigned __int128)5 * n2
        - (unsigned __int128)5 * N;

    return (seq_t)res;   /* truncates modulo 2^bitwidth if seq_t is smaller */
}
