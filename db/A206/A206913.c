/*
 * A206913  Greatest binary palindrome <= n; the binary palindrome floor
 *          function.
 * https://oeis.org/A206913
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = greatest binary palindrome ≤ n (OEIS A206913)
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bitlen_bin
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/bitlen_bin.h"

/* reverse the lowest 'width' bits of x */
static inline seq_t rev_bits(seq_t x, unsigned width)
{
    seq_t rev = 0;
    for (unsigned i = 0; i < width; ++i)
    {
        rev = (rev << 1) | (x & 1);
        x >>= 1;
    }
    return rev;
}

/* construct a palindrome from the given prefix.
   L  = total bit length of target palindrome
   half=ceil(L/2)  – number of bits in the prefix           */
static inline seq_t make_palindrome(seq_t prefix, unsigned L, unsigned half)
{
    if (L & 1) /* odd length */
    {
        seq_t rev = rev_bits(prefix >> 1, half - 1);
        return (prefix << (half - 1)) | rev;
    }
    else /* even length */
    {
        seq_t rev = rev_bits(prefix, half);
        return (prefix << half) | rev;
    }
}

/* greatest binary palindrome <= n */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    seq_t N = (seq_t)n;
    unsigned L = bitlen_bin(N);          /* total number of bits in n */
    unsigned half = (L + 1) / 2;          /* number of bits in the first half */
    seq_t prefix = N >> (L - half);      /* top 'half' bits of n */

    seq_t p = make_palindrome(prefix, L, half);
    if (p <= N)
        return p;

    /* need a smaller prefix; decrement and rebuild */
    --prefix;

    /* if the leading bit of the new prefix became 0, length drops */
    if ((prefix >> (half - 1)) == 0)
    {
        /* largest palindrome with length L-1: all ones of that length */
        return (((seq_t)1 << (L - 1)) - 1);
    }

    return make_palindrome(prefix, L, half);
}
