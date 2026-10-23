/*
 * A112851  a(n) = (n-1)*n*(n+1)*(n+2)*(2*n+1)/40.
 * https://oeis.org/A112851
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-1)*n*(n+1)*(n+2)*(2*n+1)/40
 * Range:     u8 n <= 5, u16 n <= 16, u32 n <= 153, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The first two terms are zero; for n = 0 we avoid underflow in
       (n-1) */
    if (n <= 1)
        return 0;

    /* Use a 128‑bit accumulator to avoid intermediate overflow
       (the true product is < 2^(127) for all n used in the tests). */
    unsigned __int128 N      = (unsigned __int128)n;
    unsigned __int128 product = (N - 1);
    product *= N;
    product *= (N + 1);
    product *= (N + 2);
    product *= (2 * N + 1);
    product /= 40;

    return (seq_t)product;
}
