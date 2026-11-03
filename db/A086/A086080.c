/*
 * A086080  Number of 9's in decimal expansion of triangular number n(n+1)/2.
 * https://oeis.org/A086080
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   number of 9's in the decimal expansion of n(n+1)/2
 * Range:     u8 n <= 24, u16 n <= 423, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9405 (2026-11-01)
 * Helpers:   digit_frequencies
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/digit_frequencies.h"

seq_t a(idx_t n)
{
    /* compute the triangular number n(n+1)/2 in seq_t */
    seq_t tri = (seq_t)n * ((seq_t)n + 1) / 2;

    /* count the digits of tri */
    unsigned int freq[10];
    digit_frequencies(tri, freq);

    /* a(n) is the count of digit '9' */
    return (seq_t)freq[9];
}
