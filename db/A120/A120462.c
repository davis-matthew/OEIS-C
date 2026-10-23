/*
 * A120462  Expansion of -2*x*(-3-2*x+4*x^2) / ((x-1)*(2*x+1)*(2*x-1)*(1+x)).
 * https://oeis.org/A120462
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (12*2^n - 1 - 2*(-2)^n - 3*(-1)^n)/3 Simplified to: if n
 *            even: a(n)=4*(2^n-1)/3 if n odd : a(n)=2*(4*2^n+1)/3 The
 *            implementation uses integer arithmetic only, avoiding overflow
 *            by performing division before multiplication
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 62, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute 2^n */
    seq_t pow2 = 1;
    for (idx_t i = 0; i < n; ++i)
        pow2 <<= 1;  /* multiply by 2 */

    if ((n & 1) == 0) {   /* n is even */
        seq_t tmp = pow2 - 1;   /* 2^n - 1  (divisible by 3) */
        tmp /= 3;               /* (2^n - 1)/3 */
        return tmp * 4;         /* 4 * ((2^n - 1)/3) */
    } else {                /* n is odd */
        seq_t tmp = pow2 + 1;   /* 2^n + 1 */
        tmp /= 3;               /* (2^n + 1)/3   (equals (4*2^n+1)/3 - 2*2^n?) */
        seq_t val = pow2 + tmp; /* pow2 + (pow2+1)/3 = (4*2^n+1)/3 */
        return val * 2;         /* 2 * ((4*2^n+1)/3) */
    }
}
