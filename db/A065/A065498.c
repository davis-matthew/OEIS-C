/*
 * A065498  Number of invertible n X n matrices mod 6 (i.e., over the ring
 *          Z_6).
 * https://oeis.org/A065498
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6^(n*(n-1)/2) * Π_{k=1..n} ((2^k-1)*(3^k-1))
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(0) = 1 by definition */
    if (n == 0)
        return (seq_t)1;

    seq_t prod = (seq_t)1;
    seq_t pow2 = (seq_t)1;   /* will hold 2^k */
    seq_t pow3 = (seq_t)1;   /* will hold 3^k */

    for (idx_t k = 1; k <= n; ++k)
    {
        pow2 <<= 1;                  /* multiply by 2 */
        pow3 *= (seq_t)3;            /* multiply by 3 */

        /* term = (2^k - 1) * (3^k - 1) */
        seq_t term = (pow2 - (seq_t)1) * (pow3 - (seq_t)1);
        prod *= term;
    }

    /* Compute 6^(n*(n-1)/2) by binary exponentiation */
    idx_t exp = n * (n - 1) / 2;
    seq_t pow6 = (seq_t)1;
    seq_t base = (seq_t)6;

    while (exp > 0)
    {
        if (exp & 1)
            pow6 *= base;
        base *= base;
        exp >>= 1;
    }

    return prod * pow6;
}
