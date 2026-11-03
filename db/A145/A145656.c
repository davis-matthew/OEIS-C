/*
 * A145656  a(n) = numerator of polynomial of genus 1 and level n for m = 2.
 * https://oeis.org/A145656
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, nonn, easy
 * Formula:   a(1)=0; for n≥2, a(n) = numerator of the rational sum
 *            ∑_{d=1}^{n-1} 2^(n-d)/d (the reduced numerator of the polynomial
 *            A[genus 1,level n] at m=2)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Validated: partial, pass, 23 terms through n = 23 (2026-11-01)
 * Helpers:   gcd, ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/ipow_i32.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    seq_t num = (seq_t)0;   /* current numerator   */
    seq_t den = (seq_t)1;   /* current denominator */
    seq_t pow = ipow_i32((seq_t)2, (int)(n - 1)); /* 2^(n‑1) */

    for (idx_t d = 1; d <= n - 1; ++d)
    {
        seq_t term_num = pow;              /* 2^(n‑d)          */
        seq_t term_den = (seq_t)d;          /* d                */

        /* Add fractions num/den + term_num/term_den. */
        seq_t new_num = num * term_den + den * term_num;
        seq_t new_den = den * term_den;

        seq_t g = gcd(new_num, new_den);   /* reduce the fraction */
        num = new_num / g;
        den = new_den / g;

        pow = pow / 2;                     /* next power of two   */
    }

    return num;
}
