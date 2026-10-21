/*
 * A072980  Numerator of b(n) = Sum_{k'<=n} 1/k', where k' denotes the
 *          squarefree numbers.
 * https://oeis.org/A072980
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, frac, nonn
 * Formula:   a(n) = numerator of the reduced fraction b(n) = ∑_{k'≤n} 1/k',
 *            k' running over the square‑free integers
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 25, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   gcd, is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/is_squarefree.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* out of the sequence’s domain */

    seq_t num = 0;                     /* current numerator */
    seq_t den = 1;                     /* current denominator */

    for (idx_t k = 1; k <= n; ++k) {
        if (is_squarefree(k)) {
            seq_t k_seq = (seq_t)k;            /* cast index to seq_t */
            seq_t newnum = num * k_seq + den;  /* num/k + 1/k = (num*k+den)/(den*k) */
            seq_t newden = den * k_seq;
            seq_t g = gcd(newnum, newden);
            newnum /= g;
            newden /= g;
            num = newnum;
            den = newden;
        }
    }
    return num;                          /* numerator of the reduced sum */
}
