/*
 * A035265  One half of deca-factorial numbers.
 * https://oeis.org/A035265
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (Product_{j=1..n} (10*j-8)) / 2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* In OEIS notation: 2*a(n) = Product_{j=1..n} (10*j-8) */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1; adjust for safety */
    seq_t result = 1;
    for (idx_t j = 1; j <= n; ++j)
    {
        seq_t term = (seq_t)10 * j - 8; /* 10*j-8, always even */
        result *= term;
    }
    result /= 2;                   /* divide the overall product by 2 */
    return result;
}
