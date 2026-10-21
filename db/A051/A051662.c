/*
 * A051662  House numbers: a(n) = (n+1)^3 + Sum_{i=1..n} i^2.
 * https://oeis.org/A051662
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   a(n) = (n+1)*(8*n^2 + 13*n + 6)/6
 * Range:     u8 n <= 2, u16 n <= 19, u32 n <= 889, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast index to seq_t once */
    seq_t nn = (seq_t)n;
    seq_t n1 = nn + 1;

    /* compute 8*n^2 + 13*n + 6 */
    seq_t term = (seq_t)8 * nn * nn;   /* 8*n^2 */
    term += (seq_t)13 * nn;           /* +13*n */
    term += 6;                        /* +6 */

    /* numerator (n+1)*(8*n^2 + 13*n + 6) */
    seq_t num = n1 * term;

    /* division by 6 (exact) */
    return num / 6;
}
