/*
 * A081338  a(n) = (9^n + 5^n)/2.
 * https://oeis.org/A081338
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (9^n + 5^n)/2
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Exponentiation by squaring.  Works for any arithmetic seq_t type. */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    seq_t power  = base;
    while (exp)
    {
        if (exp & (idx_t)1)
            result *= power;
        exp >>= 1;
        if (exp)
            power *= power;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t pow9 = pow_seq((seq_t)9, n);
    seq_t pow5 = pow_seq((seq_t)5, n);
    return (pow9 + pow5) / ((seq_t)2);
}
