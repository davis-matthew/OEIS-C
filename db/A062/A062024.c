/*
 * A062024  a(n) = ((n+1)^n + (n-1)^n)/2.
 * https://oeis.org/A062024
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((n+1)^n + (n-1)^n)/2
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Helper: fast exponentiation for seq_t values. */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp > 0)
    {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t term1 = pow_seq((seq_t)(n + 1), n);
    seq_t term2 = pow_seq((seq_t)(n - 1), n);
    return (term1 + term2) / 2;
}
