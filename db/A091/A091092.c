/*
 * A091092  In binary representation: minimal number of editing steps (delete,
 *          insert or substitute) to transform n into n^2.
 * https://oeis.org/A091092
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = LevenshteinDistance(binary(n), binary(n^2))
 * Range:     u8 n <= 9711, u16 n <= 9711, u32 n <= 9711, u64 n <= 9711,
 *            u128 n <= 9711, big any n
 * Helpers:   levenshtein
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   (minimal edit (insert, delete, substitute) steps in binary).  */

#include <stdint.h>
#include "../utils/levenshtein.h"

/* Compute number of bits required to represent x.  For x==0 return 1. */
static int bitlen_uint64(uint64_t x)
{
    if (x == 0) return 1;
    int len = 0;
    while (x) {
        x >>= 1;
        ++len;
    }
    return len;
}

/* Fill out[] with the bits of x, most-significant bit first.
   *len is set to the number of bits.  */
static void bits_from_uint64(uint64_t x, uint8_t *out, int *len)
{
    int l = bitlen_uint64(x);
    *len = l;
    for (int i = 0; i < l; ++i)
        out[i] = (uint8_t)((x >> (l - 1 - i)) & 1ULL);
}

seq_t a(idx_t n)
{
    /* Work in 64‑bit unsigned to avoid overflow of n^2 for n up to 10k. */
    uint64_t uns   = (uint64_t)n;
    uint64_t sq    = uns * uns;

    uint8_t bits_n[64], bits_sq[64];
    int len_n, len_sq;

    bits_from_uint64(uns, bits_n, &len_n);
    bits_from_uint64(sq,  bits_sq, &len_sq);

    int dist = levenshtein(bits_n, len_n, bits_sq, len_sq);
    return (seq_t)dist;
}
