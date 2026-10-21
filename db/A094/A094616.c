/*
 * A094616  Row sums of A094615.
 * https://oeis.org/A094616
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*3^(n+1) - 2^(n+2) - n - 1
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 18, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (equivalently, a(n) = 2(a(n-1)+3^n)+n-1 with a(0)=1) */
static seq_t _pow_seq_t(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp != 0)
    {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* convert index to seq_t type for arithmetic */
    seq_t nseq = (seq_t)n;
    /* compute 3^(n+1) and 2^(n+2) */
    seq_t pow3 = _pow_seq_t((seq_t)3, (idx_t)(n + 1));
    seq_t pow2 = _pow_seq_t((seq_t)2, (idx_t)(n + 2));

    /* a(n) = 2*pow3 - pow2 - n - 1 */
    seq_t val = (seq_t)2 * pow3;
    val -= pow2;
    val -= nseq;
    val -= 1;
    return val;
}
