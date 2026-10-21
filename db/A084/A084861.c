/*
 * A084861  Expansion of (1-3*x+4*x^2-3*x^3+x^4)/(1-2*x)^2.
 * https://oeis.org/A084861
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3*(n+3)*2^(n-4) for n >= 4 with the initial values: a(0)
 *            = 1, a(1) = 1, a(2) = 4, a(3) = 9 This is derived from the
 *            generating function (1-3*x+4*x^2-3*x^3+x^4)/(1-2*x)^2
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 29, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first four sequence values explicitly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)4;
    if (n == 3) return (seq_t)9;

    /* For n >= 4 use the closed‐form expression */
    seq_t pow2 = (seq_t)1;
    /* Shift left by (n-4) to obtain 2^(n-4) */
    pow2 <<= (n - 4);

    /* Compute 3 * (n+3) * 2^(n-4) */
    seq_t res = (seq_t)3 * ((seq_t)n + (seq_t)3) * pow2;
    return res;
}
