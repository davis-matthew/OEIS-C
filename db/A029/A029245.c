/*
 * A029245  Expansion of 1/((1-x^3)*(1-x^4)*(1-x^5)*(1-x^7)).
 * https://oeis.org/A029245
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((206*n^3+111*n^2+276*n)/720) -
 *            floor((2*n^3+n^2+2*n-3)/7)
 * Range:     u8 n <= 76, u16 n <= 249, u32 n <= 281, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* the closed‑form uses integer division with floor.  For n = 0 the second
       numerator is negative, so handle this case separately. */
    if (n == 0)
        return (seq_t)1;

    seq_t N = (seq_t)n;

    /* first floor term */
    seq_t term1 = (206 * N * N * N + 111 * N * N + 276 * N) / 720;

    /* second floor term – always non‑negative for n ≥ 1 */
    seq_t term2 = (2 * N * N * N + N * N + 2 * N - 3) / 7;

    return term1 - term2;
}
