/*
 * A169473  Number of reduced words of length n in Coxeter group on 28
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^33 = I.
 * https://oeis.org/A169473
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(n) = 28*27^(n-1) for n > 0
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* Compute 27^(n-1) using binary exponentiation */
    seq_t pow27 = (seq_t)1;
    seq_t base  = (seq_t)27;
    idx_t  exp   = n - 1;

    while (exp > 0) {
        if (exp & 1U)
            pow27 *= base;   /* multiply by current base */
        base *= base;        /* square the base */
        exp >>= 1U;
    }

    return (seq_t)28 * pow27;
}
