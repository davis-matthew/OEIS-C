/*
 * A097656  Binomial transform of A038507.
 * https://oeis.org/A097656
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} (P(n,k)+C(n,k)) = A007526(n)+2^n. Recurrence
 *            for the falling‐factorial sum S(n): S(0) = 1, S(n) = 1 + n *
 *            S(n-1) (for n ≥ 1). Then a(n) = S(n) + 2^n
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t S = 1;                     /* S(0) = 1 */
    for (idx_t i = 1; i <= n; ++i) {
        S = 1 + (seq_t)i * S;        /* S(i) = 1 + i * S(i-1) */
    }
    seq_t two_pow = pow2(n);         /* 2^n */
    return S + two_pow;              /* a(n) = S(n) + 2^n */
}
