/*
 * A053716  a(n) = 1111111 in base n.
 * https://oeis.org/A053716
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = n^6 + n^5 + n^4 + n^3 + n^2 + n + 1
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 40, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the repunit of length 7 in base n.  The argument n is 1‑based. */
seq_t a(idx_t n)
{
    seq_t r = 1;
    seq_t nn = (seq_t)n;
    /* Horner's rule: (((((1*n+1)*n+1)*n+1)*n+1)*n+1)
       which sums 1 + n + n^2 + ... + n^6 */
    for (idx_t i = 0; i < 6; ++i)
        r = r * nn + 1;
    return r;
}
