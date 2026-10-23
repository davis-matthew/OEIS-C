/*
 * A107026  Row sums of inverse of Riordan array (1/(1+x),x/(1+x)^4).
 * https://oeis.org/A107026
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3*binomial(4*n, n) - 2*∑_{k=0..n} binomial(4*n, k)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* 4*n as seq_t */
    seq_t four_n = (seq_t)4 * (seq_t)n;

    /* binomial(4n,0) = 1 */
    seq_t binom = 1;
    seq_t sum   = 1;      /* sum of binomials from k=0 to k=0 */

    for (idx_t k = 1; k <= n; ++k)
    {
        /* binomial(4n, k) = binomial(4n, k-1) * (4n-(k-1)) / k  */
        seq_t mult = four_n - (seq_t)(k-1);   /* numerator factor */
        seq_t den  = (seq_t)k;                /* denominator factor */

        /* Use 128‑bit intermediate to avoid overflow on 64‑bit builds.
           For arbitrary‑precision seq_t this cast is safe – the value
           is still representable. */
        unsigned __int128 prod = (unsigned __int128)binom * (unsigned __int128)mult;
        binom = (seq_t)(prod / den);

        sum += binom;
    }

    return (seq_t)3 * binom - (seq_t)2 * sum;
}
