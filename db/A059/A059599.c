/*
 * A059599  Expansion of g.f. (3+x)/(1-x)^6.
 * https://oeis.org/A059599
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+4,4)*(15+4*n)/5
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert n to the sequence type. */
    seq_t k = (seq_t)n;

    /* Compute binomial(n+4,4) multiplicatively.
       After each step the division is exact because (n+i) is divisible
       by i at that step. */
    seq_t res = 1;
    for (idx_t i = 1; i <= 4; ++i)
        res = res * (k + i) / i;

    /* Multiply by (15 + 4*n) and divide by 5.  The division is exact
       by the closed‑form formula. */
    seq_t factor = (seq_t)15 + (seq_t)4 * k;
    res = res * factor / 5;

    return res;
}
