/*
 * A101509  Binomial transform of tau(n) (see A000005).
 * https://oeis.org/A101509
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=0..n} binom(n,k) * tau(k+1)
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   tau_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses only integer arithmetic.  The
   divisor count tau(k+1) is computed by trial division; it
   stays tiny (≤ 256 for k ≤ 10 000) so the multiplication
   with the intermediate binomial coefficient fits safely
   into seq_t.  All operations are performed with the
   integer type seq_t, cast from the index type idx_t where
   necessary.  The code is valid C11 and C++17. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/tau_small.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t bin = 1;                 /* binom(n,0) */
    for (idx_t k = 0; k <= n; ++k) {
        seq_t tau_val = tau_small(k + 1);   /* tau(k+1) */
        sum += bin * tau_val;
        if (k == n) break;                 /* last term done */
        bin = (bin * (seq_t)(n - k)) / (seq_t)(k + 1);   /* binom(n,k+1) */
    }
    return sum;
}
