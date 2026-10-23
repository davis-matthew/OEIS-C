/*
 * A166984  a(n) = 20*a(n-1) - 64*a(n-2) for n > 1; a(0) = 1, a(1) = 20.
 * https://oeis.org/A166984
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*16^n - 4^n)/3, a(0) = 1, a(1) = 20
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t r = 1;
    while (exp)
    {
        if (exp & 1)
            r *= base;
        base *= base;
        exp >>= 1;
    }
    return r;
}

/* Computes a(n) of OEIS A166984 */
seq_t a(idx_t n)
{
    seq_t p16 = pow_seq((seq_t)16, n);   /* 16^n   */
    seq_t p4  = pow_seq((seq_t)4 , n);   /* 4^n    */
    seq_t val = (seq_t)4 * p16 - p4;    /* 4*16^n - 4^n */
    return val / 3;                     /* (4*16^n - 4^n)/3 */
}
