/*
 * A126680  Product_{i=4..n} Stirling_2(i,4).
 * https://oeis.org/A126680
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Π_{i=4..n} S(i,4) where S(i,4) is the Stirling number of
 *            the second kind
 * Range:     u8 none, u16 n <= 6, u32 n <= 8, u64 n <= 10, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For k = 4, S(n,4) = (4^n - 4·3^n + 6·2^n - 4) / 24. */

seq_t a(idx_t n)
{
    if (n < 4)
        return (seq_t)0; /* undefined for n < 4 */

    seq_t prod = (seq_t)1;

    /* Initialize powers for i = 4: 2^4, 3^4, 4^4 */
    seq_t pow2 = (seq_t)16;   /* 2^4   */
    seq_t pow3 = (seq_t)81;   /* 3^4   */
    seq_t pow4 = (seq_t)256;  /* 4^4   */

    for (idx_t i = 4; i <= n; ++i)
    {
        /* compute S(i,4) = (4^i - 4·3^i + 6·2^i - 4) / 24 */
        seq_t S4 = (pow4
                    - ((seq_t)4 * pow3)
                    + ((seq_t)6 * pow2)
                    - (seq_t)4) / ((seq_t)24);

        prod *= S4;

        /* update powers for next i+1, except after the last iteration */
        if (i != n)
        {
            pow2 *= (seq_t)2;
            pow3 *= (seq_t)3;
            pow4 *= (seq_t)4;
        }
    }

    return prod;
}
