/*
 * A166934  A positive integer n is included if the longest palindromic
 *          substring (or at least one of which that is tied for longest) in
 *          the binary representation of n consists of different valued digits
 *          (0 and 1).
 * https://oeis.org/A166934
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) is found by testing each integer sequentially for the
 *            property that its longest binary palindromic substring contains
 *            both 0 and 1. No closed‑form formula is known
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

/* Return true if seq_t x satisfies the condition. */
static bool satisfies(seq_t x)
{
    if (x == 0) return false;            /* zero never qualifies */

    /* Convert x to a bit vector (least significant bit first). */
    uint8_t bits[128];
    idx_t len = 0;
    seq_t tmp = x;
    while (tmp != 0) {
        bits[len++] = (uint8_t)(tmp & 1);
        tmp >>= 1;
    }

    /* Find the maximum palindrome length. */
    idx_t maxLen = 0;
    for (int center = 0; center < (int)len; ++center) {
        /* odd length palindromes */
        int l = center, r = center;
        while (l >= 0 && r < (int)len && bits[l] == bits[r]) {
            idx_t cur = (idx_t)(r - l + 1);
            if (cur > maxLen) maxLen = cur;
            --l; ++r;
        }
        /* even length palindromes */
        l = center; r = center + 1;
        while (l >= 0 && r < (int)len && bits[l] == bits[r]) {
            idx_t cur = (idx_t)(r - l + 1);
            if (cur > maxLen) maxLen = cur;
            --l; ++r;
        }
    }

    /* Check if any palindrome of maximum length contains both 0 and 1. */
    for (int center = 0; center < (int)len; ++center) {
        int l = center, r = center;
        while (l >= 0 && r < (int)len && bits[l] == bits[r]) {
            idx_t cur = (idx_t)(r - l + 1);
            if (cur == maxLen) {
                bool has0 = false, has1 = false;
                for (int k = l; k <= r; ++k) {
                    if (bits[k] == 0) has0 = true;
                    else has1 = true;
                    if (has0 && has1) break;
                }
                if (has0 && has1) return true;
            }
            --l; ++r;
        }
        l = center; r = center + 1;
        while (l >= 0 && r < (int)len && bits[l] == bits[r]) {
            idx_t cur = (idx_t)(r - l + 1);
            if (cur == maxLen) {
                bool has0 = false, has1 = false;
                for (int k = l; k <= r; ++k) {
                    if (bits[k] == 0) has0 = true;
                    else has1 = true;
                    if (has0 && has1) break;
                }
                if (has0 && has1) return true;
            }
            --l; ++r;
        }
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* undefined for 0, safety only */

    idx_t found = 0;
    seq_t candidate = 1;

    while (found < n) {
        if (satisfies(candidate)) {
            ++found;
            if (found == n)
                return candidate;
        }
        ++candidate;
    }
    return 0;                              /* unreachable */
}
