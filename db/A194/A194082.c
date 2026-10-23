/*
 * A194082  a(n) = Sum_{k=1..n} floor(sqrt(3)*k/2).
 * https://oeis.org/A194082
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} floor(sqrt(3)*k/2)
 * Range:     u8 n <= 4, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t kseq = (seq_t)k;
        seq_t t = (seq_t)3 * kseq * kseq;      /* t = 3*k^2 */
        seq_t s = floor_sqrt(t);              /* s = floor(sqrt(3*k^2)) */
        sum += (s >> 1);                      /* term = floor(s/2) */
    }
    return sum;
}
