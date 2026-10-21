/*
 * A061136  Number of degree-n odd permutations of order dividing 4.
 * https://oeis.org/A061136
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 15, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for the number of odd permutations of order dividing 4 (OEIS A061136).
   Let s(n) and t(n) be the e.g.f. coefficients of exp(x + x²/2 + x⁴/4) and
   exp(x – x²/2 – x⁴/4), respectively.  Put
        d(n) = s(n) – t(n)   (twice the desired count)
        e(n) = s(n) + t(n).
   Then
        d(0)=0 , d(1)=0 , d(2)=2
        e(0)=2 , e(1)=2 , e(2)=2
   and for n ≥ 3
        d(n) = d(n-1) + (n-1)*e(n-2) + (n-1)(n-2)(n-3)*e(n-4)
        e(n) = e(n-1) + (n-1)*d(n-2) + (n-1)(n-2)(n-3)*d(n-4).
   The desired term is a(n) = d(n)/2.  All calculations are performed with the
   sequence integer type seq_t; the intermediate products use 64‑bit integer
   arithmetic which is safe for all n that fit in seq_t.  For the arbitrary‑precision
   build the products are computed as seq_t multiplication. */

#include <stdint.h>

seq_t a(idx_t n) {
    /* small n handled directly */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)3;

    /* use a small ring buffer of length 5 to keep last 5 values of d and e */
    seq_t d[5] = {0, 0, 0, 0, 0};
    seq_t e[5] = {0, 0, 0, 0, 0};

    /* initialise the first three elements */
    d[0] = (seq_t)0; e[0] = (seq_t)2;          /* n = 0  */
    d[1] = (seq_t)0; e[1] = (seq_t)2;          /* n = 1  */
    d[2] = (seq_t)2; e[2] = (seq_t)2;          /* n = 2  */

    /* iterate from n = 3 up to the requested n */
    for (idx_t i = 3; i <= n; ++i) {
        idx_t im1 = (i - 1) % 5;
        idx_t im2 = (i - 2) % 5;
        idx_t im3 = (i - 3) % 5;
        idx_t im4 = (i - 4) % 5;

        /* compute d_i */
        seq_t term1_d = d[im1];
        seq_t term2_d = e[im2] * (seq_t)(i - 1);
        unsigned long long prod = (unsigned long long)(i - 1) * (i - 2) * (i - 3);
        seq_t term3_d = e[im4] * (seq_t)prod;
        seq_t new_d = term1_d + term2_d + term3_d;
        d[i % 5] = new_d;

        /* compute e_i */
        seq_t term1_e = e[im1];
        seq_t term2_e = d[im2] * (seq_t)(i - 1);
        seq_t term3_e = d[im4] * (seq_t)prod;
        seq_t new_e = term1_e + term2_e + term3_e;
        e[i % 5] = new_e;
    }

    /* result is d(n)/2 */
    return d[n % 5] / (seq_t)2;
}
