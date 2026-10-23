/*
 * A103370  Row sums of triangle A095801 (matrix square of the Narayana
 *          triangle A001263).
 * https://oeis.org/A103370
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   (n+1)*(n+2)*a(n) = 2*(5*n^2-2)*a(n-1) - 9*(n-2)*(n-1)*a(n-2);
   a(1)=1, a(2)=3. */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 0) return (seq_t)0;   /* undefined for n<1 */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)3;

    seq_t a_nm2 = (seq_t)1;  /* a(1) */
    seq_t a_nm1 = (seq_t)3;  /* a(2) */
    seq_t a_n   = 0;         /* placeholder */

    for (idx_t i = 3; i <= n; ++i) {
        /* compute coefficients in big‑int domain */
        seq_t coeff1 = (seq_t)2 * ( ( (seq_t)5 * (seq_t)i * (seq_t)i ) - (seq_t)2 );
        seq_t coeff2 = (seq_t)9 * ( (seq_t)(i - 2) ) * ( (seq_t)(i - 1) );
        seq_t denom  = ( (seq_t)(i + 1) ) * ( (seq_t)(i + 2) );

        seq_t numer = coeff1 * a_nm1 - coeff2 * a_nm2;
        a_n = numer / denom;   /* exact division */

        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_n;
}
