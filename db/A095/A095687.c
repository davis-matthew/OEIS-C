/*
 * A095687  Numbers n such that n-th Pisano number = 6*n.
 * https://oeis.org/A095687
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 0; for n >= 2, a(n) = 10 * 5^(n-2)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Efficient binary exponentiation for powers of 5. */
static seq_t pow5(idx_t e)
{
    seq_t result = (seq_t)1;
    seq_t base   = (seq_t)5;
    while (e > 0)
    {
        if (e & 1) result *= base;
        base *= base;
        e >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;
    /* n >= 2 → compute 10 * 5^(n-2) */
    return (seq_t)10 * pow5(n - 2);
}
