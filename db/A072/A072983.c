/*
 * A072983  Denominator of b(n) = Sum_{k'<=n} 1/k', where k' denotes the
 *          squarefree numbers.
 * https://oeis.org/A072983
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, frac, nonn
 * Formula:   a(n) = denominator of Σ_{k'≤n} 1/k', where k' runs over the
 *            square‑free numbers ≤ n
 * Range:     u8 n <= 10, u16 n <= 16, u32 n <= 29, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   gcd, is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* the sequence is built by
   * starting with the fraction 0/1
   * adding 1/k for each square‑free k ≤ n
   * reducing the fraction after every addition.
   * This keeps the numerator and denominator in lowest terms,
   * guaranteeing the denominator is the result. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/is_squarefree.h"

seq_t a(idx_t n)
{
    seq_t num = 0;          /* numerator of current sum */
    seq_t den = 1;          /* denominator of current sum */

    for (idx_t k = 1; k <= n; ++k)
    {
        if (!is_squarefree((seq_t)k))
            continue;

        seq_t k_seq = (seq_t)k;
        seq_t g     = gcd(den, k_seq);           /* common factor */
        seq_t d     = den / g;                   /* den/g   */
        seq_t k_g   = k_seq / g;                 /* k/g     */

        /* lcm(den,k) = den/g * k = d * k_seq */
        seq_t new_den = d * k_seq;                /* new denominator */

        /* numerator of 1/k after scaling to new denominator:
           new_den / k = d        */
        /* numerator of previous fraction after scaling:
           num * (new_den / den) = num * k_g     */
        seq_t new_num = num * k_g + d;           /* new numerator */

        /* reduce fraction */
        seq_t g2 = gcd(new_num, new_den);
        num = new_num / g2;
        den = new_den / g2;
    }

    return den;
}
