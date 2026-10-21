/*
 * A091344  a(n) = 2*3^n - 3*2^n + 1.
 * https://oeis.org/A091344
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*3^n - 3*2^n + 1
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 39, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute base^exp in seq_t using binary exponentiation.  */
static inline seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp)
    {
        if (exp & (idx_t)1)
            result = result * base;
        base = base * base;
        exp >>= (idx_t)1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* Compute the powers 2^n and 3^n. */
    seq_t pow2 = pow_seq((seq_t)2, n);
    seq_t pow3 = pow_seq((seq_t)3, n);

    /* a(n) = 2*3^n - 3*2^n + 1 */
    seq_t res = pow3 * (seq_t)2;
    res -= pow2 * (seq_t)3;
    res += (seq_t)1;
    return res;
}
