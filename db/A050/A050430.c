/*
 * A050430  Length of longest palindromic subword of (n base 2).
 * https://oeis.org/A050430
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = length of the longest palindromic subword of the binary
 *            representation of n
 * Range:     u8 n <= 16385, u16 n <= 16385, u32 n <= 16385, u64 n <= 16385,
 *            u128 n <= 16385, big any n
 * Validated: standard, pass, 121 terms through n = 9206 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Find longest palindrome length in a binary string represented by an array of bits. */
static seq_t longest_pal_binary(const uint8_t *bits, idx_t len)
{
    seq_t max_len = 1;           /* at least one bit exists for n>=1 */
    for (idx_t center = 0; center < len; ++center) {
        /* odd length palindromes */
        idx_t l = center, r = center;
        while (l < len && r < len && bits[l] == bits[r]) {
            seq_t cur = r - l + 1;
            if (cur > max_len) max_len = cur;
            if (l == 0) break;
            --l; ++r;
        }
        /* even length palindromes */
        l = center; r = center + 1;
        while (l < len && r < len && bits[l] == bits[r]) {
            seq_t cur = r - l + 1;
            if (cur > max_len) max_len = cur;
            if (l == 0) break;
            --l; ++r;
        }
    }
    return max_len;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* not used for OEIS offset 1 but safe */

    /* Determine number of bits in n. */
    idx_t len = 0;
    uint64_t tmp = n;
    while (tmp) { ++len; tmp >>= 1; }

    /* Extract bits into a static array (max ~64 bits for uint64_t). */
    uint8_t bits[128];
    for (idx_t i = 0; i < len; ++i) {
        bits[i] = (uint8_t)((n >> (len - 1 - i)) & 1ULL);
    }

    return longest_pal_binary(bits, len);
}
