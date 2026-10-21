/*
 * A064017  Number of ternary trees (A001764) with n nodes and maximal
 *          diameter.
 * https://oeis.org/A064017
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = (n+1) * 3^(n-2) for n > 1, a(1) = 1, a(2) = 3
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 19, u64 n <= 39, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is computed directly using a fast exponentiation function
   that works for any seq_t (integer or arbitrary-precision) and any idx_t. */

static inline seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp)
    {
        if (exp & 1U) result *= base;
        base *= base;
        exp >>= 1U;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    /* For n >= 2 the formula a(n) = (n+1) * 3^(n-2) holds */
    seq_t pow_three = pow_seq((seq_t)3, n - 2);
    return ((seq_t)(n + 1)) * pow_three;
}
