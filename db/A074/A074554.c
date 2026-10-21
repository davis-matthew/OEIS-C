/*
 * A074554  a(n) = 3^n + 5^n + 9^n.
 * https://oeis.org/A074554
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3^n + 5^n + 9^n
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes base^exp for arbitrary integer types seq_t and
 * unsigned index type idx_t using binary exponentiation. */
static inline seq_t pow_seq(idx_t base, idx_t exp)
{
    seq_t result = 1;
    seq_t cur = base;
    while (exp)
    {
        if (exp & 1u)
            result *= cur;
        cur *= cur;
        exp >>= 1u;
    }
    return result;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* 3^n */
    seq_t p3 = pow_seq(3, n);
    /* 5^n */
    seq_t p5 = pow_seq(5, n);
    /* 9^n */
    seq_t p9 = pow_seq(9, n);

    return p3 + p5 + p9;
}
