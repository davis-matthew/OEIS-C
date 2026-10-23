/*
 * A027993  a(n) = 1*T(n,2n) + 2*T(n,2n-1) + ... + (2n+1)*T(n,0), T given by
 *          A027926.
 * https://oeis.org/A027993
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+5)*2^n - 2*n - 4
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow2(idx_t exp)
{
    seq_t result = 1;
    seq_t base = 2;
    while (exp) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t p2 = pow2(n);
    seq_t term = ((seq_t)n + 5) * p2;
    term -= ((seq_t)(2 * n + 4));
    return term;
}
