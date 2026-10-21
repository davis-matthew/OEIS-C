/*
 * A221044  The Wiener index of the Bethe cactus lattice graph C_n defined
 *          pictorially in the Hosoya - Balasubramanian reference.
 * https://oeis.org/A221044
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = -2 + 28*3^(n-1) + (16*n-22)*3^(2*n-1)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* exponentiation by squaring for unsigned integer base and idx_t exponent */
static seq_t pow_seq_uint64(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp)
    {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* 3^(n-1) */
    seq_t p1 = pow_seq_uint64((seq_t)3, n - 1);
    /* 3^(2*n-1) */
    seq_t p2 = pow_seq_uint64((seq_t)3, (idx_t)(2 * n - 1));

    seq_t term = p1 * (seq_t)28;

    /* coefficient (16*n - 22) may be negative for n = 1 */
    int64_t coeff = (int64_t)16 * (int64_t)n - 22;
    if (coeff >= 0)
        term += p2 * (seq_t)coeff;
    else
        term -= p2 * (seq_t)(-coeff);

    term -= (seq_t)2;
    return term;
}
