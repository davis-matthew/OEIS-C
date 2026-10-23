/*
 * A025965  Expansion of 1/((1-2*x)*(1-4*x)*(1-6*x)*(1-7*x)).
 * https://oeis.org/A025965
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4^(n+1)-2^(n+1))/2 + 13*a(n-1) - 42*a(n-2) offset 0,
 *            initial terms: a(0)=1, a(1)=19
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)19;

    seq_t a_nm2 = (seq_t)1;    /* a(n-2) */
    seq_t a_nm1 = (seq_t)19;   /* a(n-1) */
    seq_t a_n;

    seq_t pow2 = (seq_t)4;     /* 2^2 */
    seq_t pow4 = (seq_t)16;    /* 4^2 */

    for (idx_t i = 2; i <= n; ++i) {
        /* update powers to 2^(i+1) and 4^(i+1) */
        pow2 = pow2 * (seq_t)2;
        pow4 = pow4 * (seq_t)4;

        seq_t diff = (pow4 - pow2) / (seq_t)2;
        a_n = diff + (seq_t)13 * a_nm1 - (seq_t)42 * a_nm2;

        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }

    return a_n;
}
