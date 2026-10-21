/*
 * A087539  First differences of A011849.
 * https://oeis.org/A087539
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(C(n+1,3)/3) - floor(C(n,3)/3)
 * Range:     u8 n <= 23, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Simplified: a(n) = (C(n,2) + (C(n,3) mod 3)) / 3  for n >= 3; a(1)=a(2)=0 */
seq_t a(idx_t n)
{
    /* Small cases */
    if (n < 3) return 0;

    /* C(n,2) = n*(n-1)/2 */
    seq_t nseq = (seq_t)n;
    seq_t c2 = nseq * (nseq - 1) / 2;

    /* Compute (C(n,3) mod 3) efficiently using modulus arithmetic */
    /* C(n,3) = n*(n-1)*(n-2)/6. Modulo 3 depends only on the product mod 18. */
    idx_t r = ((n % 18) * ((n - 1) % 18) * ((n - 2) % 18)) % 18;
    seq_t r_div6 = (seq_t)(r / 6);     /* 0 <= r_div6 <= 2 */

    /* Final formula */
    return (c2 + r_div6) / 3;
}
