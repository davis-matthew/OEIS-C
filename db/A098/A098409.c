/*
 * A098409  Expansion of 1/(sqrt(1-3*x)*sqrt(1-7*x)).
 * https://oeis.org/A098409
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = [x^n] (1+5*x+x^2)^n Recurrence: a(0)=1, a(1)=5; for n>=2:
 *            n*a(n) = 5*(2*n-1)*a(n-1) - 21*(n-1)*a(n-2);
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 45,
 *            big any n
 * Helpers:   seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)5;

    seq_t a_nm2 = (seq_t)1;   /* a(0) */
    seq_t a_nm1 = (seq_t)5;   /* a(1) */

    /* Use 128‑bit intermediate arithmetic for small sequenced types
       to avoid overflow of the transient products. */
    if (sizeof(seq_t) <= 8) {
        for (idx_t i = 2; i <= n; ++i) {
            unsigned int coeff1 = 5u * (unsigned int)(2*i - 1);
            unsigned int coeff2 = 21u * (unsigned int)(i - 1);

            unsigned __int128 prod1 = (unsigned __int128)a_nm1 * coeff1;
            unsigned __int128 prod2 = (unsigned __int128)a_nm2 * coeff2;
            unsigned __int128 numer = prod1 - prod2;

            seq_t ai = (seq_t)(numer / i);

            a_nm2 = a_nm1;
            a_nm1 = ai;
        }
    } else {
        for (idx_t i = 2; i <= n; ++i) {
            seq_t prod1 = seq_mul(a_nm1, (seq_t)(5u * (2*i - 1)));
            seq_t prod2 = seq_mul(a_nm2, (seq_t)(21u * (i - 1)));
            seq_t numer = prod1 - prod2;
            seq_t ai = numer / (seq_t)i;

            a_nm2 = a_nm1;
            a_nm1 = ai;
        }
    }

    return a_nm1;
}
