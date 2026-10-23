/*
 * A186271  a(n)=Product{k=0..n, A001333(k)}.
 * https://oeis.org/A186271
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Product_{k=0..n} sum_{j=0..floor(k/2)} binomial(k,2j)*2^j
 *            Recurrence: S_0 = 1, S_1 = 1, S_k = 2*S_{k-1} + S_{k-2} a(n) =
 *            Product_{k=0..n} S_k
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 10, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle n == 0 */
    if (n == 0)
        return (seq_t)1;

    seq_t prod = 1;

    /* S_0 = 1, add to product */
    seq_t S_prev2 = (seq_t)1;                /* S_0 */
    prod *= S_prev2;

    /* S_1 = 1, add to product */
    seq_t S_prev1 = (seq_t)1;                /* S_1 */
    prod *= S_prev1;

    for (idx_t k = 2; k <= n; ++k)
    {
        seq_t S_k = S_prev1 * (seq_t)2 + S_prev2;  /* 2*S_{k-1} + S_{k-2} */
        prod *= S_k;
        S_prev2 = S_prev1;
        S_prev1 = S_k;
    }

    return prod;
}
