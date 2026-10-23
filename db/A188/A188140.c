/*
 * A188140  Central coefficient in (1 + x + 2^n*x^2)^n.
 * https://oeis.org/A188140
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/2⌋} C(n,k) * C(n-k, n-2k) * 2^{n*k}
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   binom_n_k, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/binom_n_k.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t res = (seq_t)0;           /* running sum */
    seq_t pow_n = pow2(n);          /* 2^n  */
    seq_t pow_cur = (seq_t)1;       /* 2^{n*k}, start with k=0 */

    idx_t max_k = n / 2;
    for (idx_t k = 0; k <= max_k; ++k)
    {
        seq_t c1 = binom_n_k(n, k);                      /* C(n,k) */
        seq_t c2 = binom_n_k(n - k, n - 2 * k);          /* C(n-k, n-2k) */
        seq_t term = c1 * c2 * pow_cur;                  /* term for this k */
        res = res + term;                                /* accumulate */
        pow_cur = pow_cur * pow_n;                       /* next power: 2^{n*(k+1)} */
    }

    return res;
}
