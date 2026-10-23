/*
 * A161400  Positive integers that are palindromes (of even length) in binary,
 *          each made by concatenating two identical binary palindromes.
 * https://oeis.org/A161400
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = m*(2^k+1) where m is the n-th binary palindrome and k is
 *            the bit length of m
 * Range:     u8 n <= 6, u16 n <= 30, u32 n <= 510, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline seq_t reverse_bits(seq_t x, unsigned bits)
{
    seq_t rev = 0;
    while (bits--)
    {
        rev = (rev << 1) | (x & 1);
        x >>= 1;
    }
    return rev;
}

seq_t a(idx_t n)
{
    idx_t remaining = n;

    for (uint64_t L = 1; ; ++L)
    {
        if (L == 1)                     /* length 1 palindrome */
        {
            seq_t m = 1;
            if (--remaining == 0)
            {
                seq_t pow = ((seq_t)1 << L);
                return m * pow + m;
            }
        }
        else if ((L & 1) == 0)          /* even length */
        {
            unsigned bits = (unsigned)(L / 2);
            uint64_t start = (uint64_t)1 << (bits - 1);
            uint64_t end   = ((uint64_t)1 << bits) - 1;

            for (uint64_t half = start; ; ++half)
            {
                seq_t left = (seq_t)half;
                seq_t rev  = reverse_bits(left, bits);
                seq_t m = (left << bits) | rev;

                if (--remaining == 0)
                {
                    seq_t pow = ((seq_t)1 << L);
                    return m * pow + m;
                }
                if (half == end) break;
            }
        }
        else                               /* odd length */
        {
            unsigned bits = (unsigned)(L / 2);
            uint64_t start = (uint64_t)1 << (bits - 1);
            uint64_t end   = ((uint64_t)1 << bits) - 1;

            for (uint64_t half = start; ; ++half)
            {
                seq_t left = (seq_t)half;
                seq_t rev  = reverse_bits(left, bits);

                for (int center = 0; center <= 1; ++center)
                {
                    seq_t m = (left << (bits + 1)) | ((seq_t)center << bits) | rev;

                    if (--remaining == 0)
                    {
                        seq_t pow = ((seq_t)1 << L);
                        return m * pow + m;
                    }
                }
                if (half == end) break;
            }
        }
    }
}
