/*
 * A056595  Number of nonsquare divisors of n.
 * https://oeis.org/A056595
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = τ(n) − τ(∏_{p^e‖n} p^{⌊e/2⌋}) where τ(m) is the number of
 *            divisors of m. For n = 1 the sequence is defined to be 0
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)   /* behaviour below the offset is irrelevant */
        return 0;

    idx_t tmp = n;
    seq_t tau        = (seq_t)1;      /* product of (e_i + 1)   */
    seq_t tau_square = (seq_t)1;      /* product of (⌊e_i/2⌋ + 1) */

    for (idx_t p = 2; p <= tmp / p; ++p)
    {
        if (tmp % p == 0)
        {
            idx_t exp = 0;
            while (tmp % p == 0)
            {
                tmp /= p;
                ++exp;
            }
            tau        *= (seq_t)(exp + 1);
            tau_square *= (seq_t)(exp / 2 + 1);
        }
    }

    /* Remaining prime factor, if any */
    if (tmp > 1)
    {
        tau        *= (seq_t)2;       /* exponent 1 → (1+1) */
        /* (⌊1/2⌋ + 1) = 1, so tau_square unchanged */
    }

    return tau - tau_square;
}
