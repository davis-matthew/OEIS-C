/*
 * A029205  Expansion of 1/((1-x^2)*(1-x^5)*(1-x^8)*(1-x^10)).
 * https://oeis.org/A029205
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n^3+45*n^2+512*n+4272)/4800 - * (n^2+25*n+92)*(n
 *            mod 2)/320 + * (n+6)*((2*n^2+3) mod 5)/50)
 * Range:     u8 none, u16 n <= 25, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N   = (seq_t)n;
    seq_t N2  = N * N;
    seq_t N3  = N2 * N;

    /* First term numerator */
    seq_t num1 = N3 + (seq_t)45 * N2 + (seq_t)512 * N + (seq_t)4272;

    /* Second term numerator multiplied by (n mod 2) */
    seq_t n_mod2 = N % (seq_t)2;
    seq_t num2 = (N2 + (seq_t)25 * N + (seq_t)92) * n_mod2;

    /* Third term numerator */
    seq_t mod5 = ((seq_t)2 * N2 + (seq_t)3) % (seq_t)5;
    seq_t num3 = (N + (seq_t)6) * mod5;

    /* Combine with common denominator 4800:
     *  A   =  num1
     *  B   = 15 * num2  (since 4800/320 = 15)
     *  C   = 96 * num3  (since 4800/50  = 96)   */
    seq_t total = num1 - (seq_t)15 * num2 + (seq_t)96 * num3;

    return total / (seq_t)4800;
}
