/*
 * A093460  a(n) = 2n^(n-1) - 1.
 * https://oeis.org/A093460
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*n^(n-1) - 1
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow_seq_t(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp) {
        if (exp & 1) {
            result *= base;
        }
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* offset of the sequence is 1; return 0 for n == 0 (undefined). */
    if (n == 0) {
        return 0;
    }
    seq_t base = (seq_t)n;                /* cast index to seq_t */
    seq_t power = pow_seq_t(base, n - 1); /* compute n^(n-1) */
    seq_t res = power * ((seq_t)2);        /* 2 * n^(n-1) */
    return res - ((seq_t)1);              /* subtract 1 */
}
