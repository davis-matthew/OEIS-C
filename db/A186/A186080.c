/*
 * A186080  Fourth powers that are palindromic in base 10.
 * https://oeis.org/A186080
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1)=0, a(2)=1; for n≥3, a(n) = (10^(n-2)+1)^4
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 11,
 *            big any n
 * Helpers:   pow10_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow10_seq.h"

/* Helper to compute 10^e as a seq_t using repeated multiplication.
   This works for any seq_t (fixed or arbitrary precision). */

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)0;
    if (n == 2)
        return (seq_t)1;
    /* n ≥ 3: compute (10^(n-2)+1)^4 */
    seq_t base = pow10_seq(n - 2);
    base += (seq_t)1;
    seq_t sq = base * base;
    return sq * sq;
}
