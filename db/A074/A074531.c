/*
 * A074531  a(n) = 2^n + 3^n + 9^n.
 * https://oeis.org/A074531
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^n + 3^n + 9^n
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;          /* 1 cast is safe for small constant */
    seq_t cur = base;
    while (exp > 0) {
        if (exp & 1) result *= cur;
        exp >>= 1;
        if (exp) cur *= cur;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* compute 2^n */
    seq_t pow2 = pow_seq((seq_t)2, n);

    /* compute 3^n */
    seq_t pow3 = pow_seq((seq_t)3, n);

    /* compute 9^n as (3^n)^2 */
    seq_t pow9 = pow3 * pow3;

    return pow2 + pow3 + pow9;
}
