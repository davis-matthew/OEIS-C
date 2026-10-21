/*
 * A084363  a(n) = n^(n+1) - (n-1)^n.
 * https://oeis.org/A084363
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n^(n+1) - (n-1)^n (offset 1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow_seq(idx_t base, idx_t exp)
{
    seq_t result = 1;
    seq_t b = base;
    while (exp) {
        if (exp & 1u) result *= b;
        b *= b;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* assumes n >= 1 */
    seq_t pow1 = pow_seq(n, n + 1);     /* n^(n+1) */
    seq_t pow2 = pow_seq(n - 1, n);     /* (n-1)^n */
    return pow1 - pow2;
}
