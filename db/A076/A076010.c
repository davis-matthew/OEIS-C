/*
 * A076010  Fourth column of triangle A075504.
 * https://oeis.org/A076010
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (-9^n + 24*18^n - 81*27^n + 64*36^n) / 6
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 5, u64 n <= 11, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow_seq_t_local(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp)
    {
        if (exp & 1)
            result *= base;
        exp >>= 1;
        if (exp) /* avoid unnecessary squaring when exp becomes 0 */
            base *= base;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t p9  = pow_seq_t_local((seq_t)9,  n);
    seq_t p18 = pow_seq_t_local((seq_t)18, n);
    seq_t p27 = pow_seq_t_local((seq_t)27, n);
    seq_t p36 = pow_seq_t_local((seq_t)36, n);

    seq_t big   = (seq_t)64 * p36 + (seq_t)24 * p18;
    seq_t small = (seq_t)81 * p27 + p9;

    return (big - small) / ((seq_t)6);
}
