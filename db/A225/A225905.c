/*
 * A225905  Denominator of Sum_{k=1..n} 1/L(k) where L(n) is the n-th Lucas
 *          number (A000204).
 * https://oeis.org/A225905
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = denominator of 1/L(1) + 1/L(2) + … + 1/L(n) where L(k) is
 *            the k‑th Lucas number. The function keeps a reduced fraction
 *            num/den and at each step adds 1/L(k) to it, then reduces the
 *            fraction by gcd(num,den)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 14, u128 n <= 18,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>                /* for idx_t definition */
#include "../utils/gcd.h"          /* gcd(seq_t, seq_t) */

seq_t a(idx_t n)
{
    /* n is 1‑based; a(1) = 1 */
    if (n <= 1) return 1;

    seq_t num = 1;       /* numerator of the sum  */
    seq_t den = 1;       /* denominator of the sum  */

    /* Initialize Lucas numbers: L(1)=1, L(2)=3 */
    seq_t L_prevprev = 1;  /* L(k-2)   */
    seq_t L_prev     = 3;  /* L(k-1)   */

    for (idx_t k = 2; k <= n; ++k) {
        seq_t Lk = L_prev;          /* current Lucas number */

        /* add 1/Lk to the fraction:   num/den + 1/Lk */
        seq_t den_before = den;      /* keep old denominator for formula */
        den = den * Lk;              /* new denominator before reduction */
        num = num * Lk + den_before;/* numerator before reduction */

        /* reduce the fraction */
        seq_t g = gcd(num, den);
        num /= g;
        den /= g;

        /* update Lucas numbers for next iteration */
        seq_t next = L_prev + L_prevprev; /* L(k+1) = L(k) + L(k-1) */
        L_prevprev = L_prev;
        L_prev     = next;
    }

    return den;   /* denominator of the reduced sum */
}
