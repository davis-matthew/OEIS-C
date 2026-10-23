/*
 * A025935  Expansion of 1/((1-2*x)*(1-3*x)*(1-5*x)*(1-11*x)).
 * https://oeis.org/A025935
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^(n+1) - 2^(n+1) + 16*a(n-1) - 55*a(n-2); with base
 *            values a(0)=1, a(1)=21
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)1;
    if (n == 1u) return (seq_t)21;

    seq_t a_prev2 = (seq_t)1;   /* a(0) */
    seq_t a_prev1 = (seq_t)21;  /* a(1) */
    seq_t pow2   = (seq_t)4;    /* 2^(1+1)  */
    seq_t pow3   = (seq_t)9;    /* 3^(1+1)  */

    for (idx_t i = 2; i <= n; ++i) {
        pow2 *= (seq_t)2;      /* 2^(i+1)  */
        pow3 *= (seq_t)3;      /* 3^(i+1)  */
        seq_t term = pow3 - pow2 + (seq_t)16 * a_prev1 - (seq_t)55 * a_prev2;
        a_prev2 = a_prev1;
        a_prev1 = term;
    }
    return a_prev1;
}
