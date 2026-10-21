/*
 * A090024  Number of distinct lines through the origin in the n-dimensional
 *          lattice of side length 8.
 * https://oeis.org/A090024
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 9^n - 5^n - 3^n - 2^n + 2
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(0) = 0, a(1) = 1, ...  (closed‑form implementation) */

static seq_t pow_seq(idx_t exp, seq_t base)
{
    seq_t result = (seq_t)1;
    while (exp)
    {
        if (exp & 1U)
            result *= base;
        base *= base;
        exp >>= 1U;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t p9 = pow_seq(n, (seq_t)9);
    seq_t p5 = pow_seq(n, (seq_t)5);
    seq_t p3 = pow_seq(n, (seq_t)3);
    seq_t p2 = pow_seq(n, (seq_t)2);
    return p9 - p5 - p3 - p2 + (seq_t)2;
}
