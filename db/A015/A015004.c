/*
 * A015004  q-factorial numbers for q=5.
 * https://oeis.org/A015004
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Product_{k=1..n} ((5^k - 1) / 4)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 7, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 1;
    seq_t power  = 5;   /* power = 5^k, starts with k = 1          */
    for (idx_t k = 1; k <= n; ++k) {
        seq_t term   = (power - 1) / 4;  /* (5^k - 1) / 4  */
        result *= term;                  /* multiply into product            */
        power *= 5;                      /* prepare 5^{k+1} for next iteration */
    }
    return result;
}
