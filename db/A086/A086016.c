/*
 * A086016  Number of 8's in decimal expansion of n^2.
 * https://oeis.org/A086016
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = number of '8' digits in decimal expansion of n^2
 * Range:     u8 n <= 16, u16 n <= 322, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_frequencies
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/digit_frequencies.h"

seq_t a(idx_t n)
{
    seq_t sq = (seq_t)n * (seq_t)n;
    unsigned int freq[10] = {0};
    digit_frequencies(sq, freq);
    return (seq_t)freq[8];
}
