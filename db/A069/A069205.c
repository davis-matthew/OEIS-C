/*
 * A069205  a(n) = Sum_{k=1..n} 2^bigomega(k).
 * https://oeis.org/A069205
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=1..n} 2^bigomega(k) (offset 1)
 * Range:     u8 n <= 43, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   bigomega, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"
#include "../utils/bigomega.h"

seq_t a(idx_t n)
{
    if (n == 0)  /* undefined for the sequence, but safe for any call */
        return (seq_t)0;

    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t w = bigomega(k);               /* total number of prime factors (with multiplicity) of k */
        seq_t term = pow2((idx_t)w);         /* 2^w */
        sum += term;                         /* accumulate the sum */
    }
    return sum;
}
