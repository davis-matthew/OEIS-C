/*
 * A221714  Numbers written in base 2 with digits rearranged to be in
 *          decreasing order.
 * https://oeis.org/A221714
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = decimal integer whose digits are the bits of n written in
 *            binary, sorted in decreasing order (i.e., all 1‑bits first
 *            followed by all 0‑bits)
 * Range:     u8 n <= 7, u16 n <= 31, u32 n <= 1023, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* For n > 0:
       bitlen = floor_log2(n) + 1  (number of bits in n)
       popcnt = number of 1‑bits in n
       shift  = bitlen - popcnt   (number of trailing zeros after sorting)
   The required decimal number has popcnt “1” digits followed by shift
   “0” digits.  We build it by repeatedly multiplying by 10 and
   adding the next digit.  For n = 0 the result is 0. */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    /* Count the number of 1-bits in n. */
    unsigned int popcnt = __builtin_popcountll((unsigned long long)n);

    /* Compute the number of bits needed to represent n. */
    unsigned int bitlen = 64 - __builtin_clzll((unsigned long long)n);

    /* After sorting, the ones are shifted to the leftmost positions. */
    unsigned int shift = bitlen - popcnt;

    seq_t val = (seq_t)0;
    /* Append popcnt “1” digits. */
    for (unsigned int i = 0; i < popcnt; ++i)
        val = val * ((seq_t)10) + (seq_t)1;
    /* Append shift “0” digits. */
    for (unsigned int i = 0; i < shift; ++i)
        val = val * ((seq_t)10);

    return val;
}
