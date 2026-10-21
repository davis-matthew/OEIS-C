/*
 * A057863  a(n) = Product_{k=1..n} (2k-1)!!.
 * https://oeis.org/A057863
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Product_{k=1..n} (2*k-1)!!
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 7, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 1;   /* a(0) = 1 */
    seq_t D       = 1;   /* (2*k-1)!! of the previous step */

    for (idx_t k = 1; k <= n; ++k)
    {
        /* Compute (2*k-1)!! for this step */
        D = D * ((seq_t)2 * k - 1);

        /* Multiply into the running product */
        result = result * D;
    }

    return result;
}
