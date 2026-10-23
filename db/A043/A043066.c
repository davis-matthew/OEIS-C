/*
 * A043066  a(n) = (s(n)+6)/8, where s(n) = n-th base 8 palindrome that starts
 *          with 2.
 * https://oeis.org/A043066
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (A043022(n)+6)/8, where A043022(n) is the n‑th base‑8
 *            palindrome that starts with digit 2
 * Range:     u8 n <= 10, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   pow8_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow8_uint.h"

seq_t a(idx_t n)
{
    /* 1. Find the length of the palindrome that contains the n‑th element. */
    idx_t length = 1;
    idx_t cumulative = 0;          /* count of palindromes of all shorter lengths */

    while (1)
    {
        idx_t count;
        if (length == 1)
            count = 1;
        else if (length & 1)        /* odd length */
            count = (idx_t)pow8_uint((length - 1) / 2);
        else                        /* even length */
            count = (idx_t)pow8_uint((length / 2) - 1);

        if (cumulative + count >= n)
            break;

        cumulative += count;
        ++length;
    }

    /* 2. Determine the offset (1‑based) within the current length group. */
    idx_t offset = n - cumulative;               /* 1 … count  */
    idx_t free_count = (length & 1) ? (length - 1) / 2 : length / 2 - 1;

    /* 3. Convert (offset‑1) to base‑8 digits that determine the interior. */
    uint8_t digits[20] = {0};  /* free_count is never larger than 10 for n≤10000 */
    idx_t rem = offset - 1;
    for (idx_t i = 0; i < free_count; ++i)
    {
        digits[free_count - 1 - i] = (uint8_t)(rem % 8);
        rem /= 8;
    }

    /* 4. Reconstruct the base‑8 palindrome and convert it to decimal. */
    seq_t s = 0;
    for (idx_t pos = 0; pos < length; ++pos)
    {
        seq_t d;
        if (pos == 0 || pos == length - 1)
            d = 2;                      /* first and last digits are fixed to 2 */
        else
        {
            idx_t mirror = (pos < length - 1 - pos) ? pos : (length - 1 - pos);
            d = digits[mirror - 1];      /* mirror index is at least 1 */
        }
        s = s * 8 + d;
    }

    /* 5. Compute the final term of the sequence. */
    return (s + 6) / 8;
}
