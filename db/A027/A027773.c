/*
 * A027773  a(n) = (n+1)*binomial(n+1,13).
 * https://oeis.org/A027773
 *
 * Offset:    12    (a(12) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)*binomial(n+1,13)
 * Range:     u8 n <= 13, u16 n <= 16, u32 n <= 29, u64 n <= 123,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 41 terms through n = 933 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t res = binomial(n + 1, 13);
    return res * (seq_t)(n + 1);
}
