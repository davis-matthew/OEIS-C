/*
 * A206919  Sum of binary palindromes <= n.
 * https://oeis.org/A206919
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = sum of all binary palindrome integers <= n
 * Range:     u8 n <= 50, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/* Return the number of bits needed to represent the unsigned integer x. */
static size_t bitlen_idx(idx_t x)
{
    size_t len = 0;
    while (x) {
        ++len;
        x >>= 1;
    }
    return len;
}

/* Test whether the binary representation of x is a palindrome. */
static bool is_bin_pal(idx_t x)
{
    size_t len = bitlen_idx(x);
    for (size_t i = 0; i + 1 < len; ++i) {
        bool low  = ((x >> i) & (idx_t)1) != 0;
        bool high = ((x >> (len - 1 - i)) & (idx_t)1) != 0;
        if (low != high)
            return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t i = 0; i <= n; ++i) {
        if (is_bin_pal(i))
            sum += (seq_t)i;
    }
    return sum;
}
