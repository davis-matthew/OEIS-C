/*
 * A081042  7th binomial transform of (1,6,0,0,0,0,0,0,...).
 * https://oeis.org/A081042
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (6*n + 7) * 7^(n-1) (a(0)=1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    seq_t pow   = (seq_t)1;
    seq_t base  = (seq_t)7;
    idx_t exp   = n - 1;

    while (exp) {
        if (exp & 1) pow = pow * base;
        base = base * base;
        exp >>= 1;
    }

    seq_t factor = (seq_t)(6 * n + 7);
    return factor * pow;
}
