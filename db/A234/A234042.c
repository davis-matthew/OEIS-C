/*
 * A234042  a(n) = binomial(n+4,4)*gcd(n,5)/5.
 * https://oeis.org/A234042
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+4,4)*gcd(n,5)/5
 * Range:     u8 n <= 1, u16 n <= 13, u32 n <= 563, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nseq = (seq_t)n;
    seq_t prod = (nseq + 4);
    prod *= (nseq + 3);
    prod *= (nseq + 2);
    prod *= (nseq + 1);
    prod /= 24;                      /* binomial(n+4,4) */
    /* Multiply by gcd(n,5)/5: if n is a multiple of 5, gcd=5 → factor 1
       otherwise gcd=1 → divide by 5 (binomial always divisible by 5 in that case) */
    if ((n % 5) == 0)
        return prod;
    else
        return prod / 5;
}
