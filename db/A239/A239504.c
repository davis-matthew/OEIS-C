/*
 * A239504  Number of digits in the decimal expansion of n^10 (A008454).
 * https://oeis.org/A239504
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = floor(10*log10(n))+1 for n>0, a(0)=1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 9, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Validated: partial, pass, 67 terms through n = 66 (2026-11-01)
 * Helpers:   ipow_u64_idx, num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/ipow_u64_idx.h"
#include "../utils/num_digits.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    seq_t p = ipow_u64_idx((uint64_t)n, 10);
    return (seq_t)num_digits(p);
}
