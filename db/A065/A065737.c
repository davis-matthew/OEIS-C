/*
 * A065737  Largest square <= binomial(n,2).
 * https://oeis.org/A065737
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor(sqrt(n*(n-1)/2))^2
 * Range:     u8 n <= 23, u16 n <= 362, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/floor_sqrt_seq.h"

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t bin = nn * (nn - ((seq_t)1)) / 2;
    seq_t s   = floor_sqrt_seq(bin);
    return s * s;
}
