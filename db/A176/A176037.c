/*
 * A176037  a(n) = n!*(n+1)!*(n+2)!.
 * https://oeis.org/A176037
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n! * (n+1)! * (n+2)!
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute (n+2)! once */
    seq_t fact_np2 = 1;
    idx_t limit = n + 2;
    for (idx_t i = 1; i <= limit; ++i)
        fact_np2 *= (seq_t)i;

    /* derive (n+1)! and n! by dividing by small integers */
    seq_t fact_np1 = fact_np2 / (seq_t)(n + 2);
    seq_t fact_n   = fact_np1 / (seq_t)(n + 1);

    /* return the product of the three factorials */
    return fact_n * fact_np1 * fact_np2;
}
