/*
 * A085277  Expansion of (1+x)^2/((1-2x)(1-3x)).
 * https://oeis.org/A085277
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (16/3) * 3^n - (9/2) * 2^n (For n=0 a(0)=1; for n=1
 *            a(1)=7; the formula holds for n>=1 when 0^n is interpreted as 0
 *            for n>0.)
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 18, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle small indices directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)7;

    /* Compute 3^n and 2^n. */
    seq_t pow3 = (seq_t)1;
    seq_t pow2 = (seq_t)1;
    for (idx_t i = 0; i < n; ++i) {
        pow3 *= (seq_t)3;
        pow2 *= (seq_t)2;
    }

    /* For n >= 1, 3^n is divisible by 3 and 2^n by 2. */
    seq_t pow3_div3 = pow3 / (seq_t)3;      /* 3^(n-1) */
    seq_t term1 = pow3_div3 * (seq_t)16;    /* (16/3) * 3^n */

    seq_t pow2_div2 = pow2 / (seq_t)2;      /* 2^(n-1) */
    seq_t term2 = pow2_div2 * (seq_t)9;     /* (9/2) * 2^n */

    return term1 - term2;
}
