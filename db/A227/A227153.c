/*
 * A227153  Product of nonzero digits of n in factorial base.
 * https://oeis.org/A227153
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2757, u16 n <= 5040, u32 n <= 5040, u64 n <= 5040,
 *            u128 n <= 5040, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A227153: product of nonzero digits of n in factorial base */
seq_t a(idx_t n)
{
    seq_t product = 1;          /* empty product for n = 0 should be 1 */
    idx_t  temp = n;
    for (idx_t i = 1; temp > 0; ++i)
    {
        idx_t divisor = i + 1;              /* base for this digit */
        idx_t digit   = temp % divisor;     /* current factorial-base digit */
        if (digit != 0)
            product *= (seq_t)digit;        /* multiply nonzero digit */
        temp /= divisor;                   /* move to next higher factorial place */
    }
    return product;
}
