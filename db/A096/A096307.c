/*
 * A096307  E.g.f.: exp(x)/(1-x)^6.
 * https://oeis.org/A096307
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ∑_{k=0}^{n} binomial(n,k) * (k+5)!/5!
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (= (k+5)!/5! = ∏_{i=1}^{k} (i+5) )                              */

seq_t a(idx_t n)
{
    /* binomial coefficient C(n,k)   */
    /* product (k+5)!/5!             */
    seq_t sum = 1;          /* term for k = 0                 */
    seq_t bin  = 1;          /* C(n,0)                          */
    seq_t prod = 1;          /* ∏_{i=1}^{0} (i+5) = 1           */

    for (idx_t k = 1; k <= n; ++k)
    {
        /* bin = bin * (n - k + 1) / k   – exact division          */
        bin = (bin * (seq_t)(n - k + 1)) / (seq_t)k;

        /* prod = prod * (k + 5)                                          */
        prod *= (seq_t)(k + 5);

        /* accumulate the term                                          */
        sum += bin * prod;
    }
    return sum;
}
