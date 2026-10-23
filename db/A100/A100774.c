/*
 * A100774  a(n) = 2*(3^n - 1).
 * https://oeis.org/A100774
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*(3^n - 1)
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 39, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow3_exp(idx_t e)
{
    seq_t result = 1;
    seq_t base   = 3;
    while (e) {
        if (e & (idx_t)1) result *= base;
        base *= base;
        e >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t pow3 = pow3_exp(n);  /* 3^n */
    pow3 -= (seq_t)1;          /* 3^n - 1 */
    pow3 <<= 1;                /* * 2 */
    return pow3;
}
