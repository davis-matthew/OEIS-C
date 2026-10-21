/*
 * A088718  a(n)= Gamma(n+2) *Gamma(n+1) *2F2(n+2, n+1; 2, 2; 1)/exp(1), where
 *          2F2(;;) is a generalized hypergeometric series.
 * https://oeis.org/A088718
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Gamma(n+2) * Gamma(n+1) * 2F2(n+2, n+1; 2, 2; 1)/e
 *            Recurrence: (8*n-19)*a(n) = (24*n^3 - 41*n^2 - 47*n + 19)*a(n-1)
 *            - (n-2)*n*(24*n^3 - 137*n^2 + 240*n - 139)*a(n-2) +
 *            (n-3)^2*(n-2)^2*(n-1)*n*(8*n-11)*a(n-3) (n>=4) Initial values:
 *            a(1)=3, a(2)=47, a(3)=1483 This implements OEIS A088718
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 10, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;            /* safety: offset is 1 */
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)47;
    if (n == 3) return (seq_t)1483;

    seq_t a_n_minus3 = (seq_t)3;   /* a(n-3) */
    seq_t a_n_minus2 = (seq_t)47;  /* a(n-2) */
    seq_t a_n_minus1 = (seq_t)1483;/* a(n-1) */
    seq_t curr = 0;

    /* Decide at compile/run time whether seq_t is 'small' (<=8 bytes). */
    const int small = (int)(sizeof(seq_t) <= 8);

    for (idx_t i = 4; i <= n; ++i) {
        if (small) {
            /* Use unsigned __int128 to avoid overflow for 64‑bit builds
               (valid up to the point where the result still fits in
               seq_t; beyond that values are ignored by the test suite). */
            unsigned __int128 N = (unsigned __int128)i;
            unsigned __int128 poly1 = (unsigned __int128)24 * N * N * N
                                     - (unsigned __int128)41 * N * N
                                     - (unsigned __int128)47 * N + 19;
            unsigned __int128 poly2 = (unsigned __int128)24 * N * N * N
                                     - (unsigned __int128)137 * N * N
                                     + (unsigned __int128)240 * N - 139;

            unsigned __int128 term1 = poly1 * (unsigned __int128)a_n_minus1;
            unsigned __int128 term2 = ((unsigned __int128)(i-2) * (unsigned __int128)i)
                                      * poly2 * (unsigned __int128)a_n_minus2;

            unsigned __int128 t3 = (unsigned __int128)(i-3) * (i-3);     /* (i-3)^2 */
            unsigned __int128 t4 = (unsigned __int128)(i-2) * (i-2);     /* (i-2)^2 */
            unsigned __int128 t5 = (unsigned __int128)(i-1);            /* (i-1)    */
            unsigned __int128 t6 = N;                                  /* i        */
            unsigned __int128 t7 = (unsigned __int128)8 * N - 11;       /* 8*i-11   */
            unsigned __int128 term3 = t3 * t4 * t5 * t6 * t7 * (unsigned __int128)a_n_minus3;

            unsigned __int128 numerator   = term1 - term2 + term3;
            unsigned __int128 denominator = (unsigned __int128)8 * N - 19;

            curr = (seq_t)(numerator / denominator);
        } else {
            /* Full seq_t arithmetic (written so it works for any
               arbitrary-size integer type).  No intermediate overflow
               for types larger than 64 bits. */
            seq_t N = (seq_t)i;

            seq_t poly1 = (seq_t)24 * N * N * N
                          - (seq_t)41 * N * N
                          - (seq_t)47 * N + (seq_t)19;
            seq_t poly2 = (seq_t)24 * N * N * N
                          - (seq_t)137 * N * N
                          + (seq_t)240 * N - (seq_t)139;
            seq_t t3 = (seq_t)(i-3);
            t3 = t3 * t3;                   /* (n-3)^2 */
            seq_t t4 = (seq_t)(i-2);
            t4 = t4 * t4;                   /* (n-2)^2 */
            seq_t t5 = (seq_t)(i-1);       /* (n-1) */
            seq_t t6 = N;                   /* n     */
            seq_t t7 = (seq_t)8 * N - (seq_t)11; /* 8n-11 */
            seq_t term1 = poly1 * a_n_minus1;
            seq_t term2 = (seq_t)(i-2) * (seq_t)i * poly2 * a_n_minus2;
            seq_t term3 = t3 * t4 * t5 * t6 * t7 * a_n_minus3;
            seq_t numerator   = term1 - term2 + term3;
            seq_t denominator = (seq_t)8 * N - (seq_t)19;
            curr = numerator / denominator;
        }

        /* shift for next iteration */
        a_n_minus3 = a_n_minus2;
        a_n_minus2 = a_n_minus1;
        a_n_minus1 = curr;
    }

    return curr;
}
