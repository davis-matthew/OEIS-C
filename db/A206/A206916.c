/*
 * A206916  Index of the least binary palindrome >=n; also the "upper inverse"
 *          of A006995.
 * https://oeis.org/A206916
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = min(m | A006995(m) >= n) + 1 (index of the first binary
 *            palindrome ≥ n)
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   reverse_bits_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/reverse_bits_uint64.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* Bit length of n */
    uint64_t v = (uint64_t)n;
    unsigned L = 0;
    while (v)
    {
        ++L;
        v >>= 1;
    }

    unsigned k = (L + 1) >> 1;          /* number of bits that determine the palindrome */
    unsigned right_bits = L - k;
    uint64_t left = (uint64_t)n >> right_bits;
    uint64_t tmp_left = (L & 1) ? (left >> 1) : left;
    uint64_t rev = reverse_bits_uint64(tmp_left, right_bits);
    uint64_t pal_candidate = (left << right_bits) | rev;

    if (pal_candidate < (uint64_t)n)   /* need a larger palindrome */
    {
        ++left;                        /* increase the prefix */

        if ((left >> k) != 0)          /* overflow, go to next length */
        {
            ++L;
            k = (L + 1) >> 1;
            right_bits = L - k;
            left = 1ULL << (k - 1);
            tmp_left = (L & 1) ? (left >> 1) : left;
            rev = reverse_bits_uint64(tmp_left, right_bits);
            pal_candidate = (left << right_bits) | rev;
        }
        else /* rebuild with increased prefix */
        {
            tmp_left = (L & 1) ? (left >> 1) : left;
            rev = reverse_bits_uint64(tmp_left, right_bits);
            pal_candidate = (left << right_bits) | rev;
        }
    }

    /* Count palindromes of smaller length (including zero) */
    unsigned long long rank = 1ULL; /* account for 0 */

    for (unsigned len = 1; len < L; ++len)
    {
        unsigned half = (len + 1) >> 1;
        rank += 1ULL << (half - 1);
    }

    /* Count same length palindromes with smaller prefix */
    uint64_t min_prefix = 1ULL << (k - 1);
    rank += (unsigned long long)(left - min_prefix);

    /* Return rank + 1 (indices are 1‑based) */
    return (seq_t)(rank + 1ULL);
}
