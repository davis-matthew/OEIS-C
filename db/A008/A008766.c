/*
 * A008766  Expansion of (1+x^5)/((1-x)*(1-x^2)*(1-x^3)*(1-x^4)).
 * https://oeis.org/A008766
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = round((2*(n+5)^3 - 15*(n+5)^2 + 60*(n+5) - (n+5 mod
 *            2==0?110:65))/144)
 * Range:     u8 n <= 1, u16 n <= 29, u32 n <= 1416, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t N = n + 5;                 /* N = n+5 */
    seq_t NN = (seq_t)N;             /* promote N to seq_t */
    seq_t N2 = NN * NN;              /* N^2 */
    seq_t N3 = N2 * NN;              /* N^3 */
    seq_t num = 2 * N3 - 15 * N2 + 60 * NN;   /* 2*N^3 - 15*N^2 + 60*N */
    if ((N & 1) == 0)                /* N even */
        num -= 110;
    else
        num -= 65;                   /* N odd */
    return (num + 72) / 144;         /* round(num/144) == floor((num+72)/144) */
}
