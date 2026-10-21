/*
 * A215244  a(n) is the number of ways of writing the binary expansion of n as
 *          a product (or concatenation) of palindromes.
 * https://oeis.org/A215244
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = number of ways to write the binary expansion of n as a
 *            concatenation of binary palindromes
 * Range:     u8 n <= 510, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* maximum number of bits for a 64‑bit integer */
    const size_t MAX_BITS = 64;

    /* arrays of fixed size – no variable‑length arrays are used. */
    bool pal[MAX_BITS][MAX_BITS];
    unsigned char bits[MAX_BITS];
    seq_t dp[MAX_BITS + 1];

    /* determine length of the binary representation (0 for n==0) */
    size_t len = 0;
    idx_t tmp = n;
    while (tmp) {
        ++len;
        tmp >>= 1;
    }

    /* store bits in most significant bit first order */
    for (size_t i = 0; i < len; ++i) {
        bits[i] = (unsigned char)((n >> (len - 1 - i)) & 1);
    }

    /* initialize palindrome table */
    for (size_t i = 0; i < len; ++i) {
        pal[i][i] = true;                         /* single bit */
        if (i + 1 < len) {
            pal[i][i + 1] = (bits[i] == bits[i + 1]);   /* two bits */
        }
    }
    for (size_t L = 3; L <= len; ++L) {               /* length ≥ 3 */
        for (size_t i = 0; i + L <= len; ++i) {
            size_t j = i + L - 1;
            pal[i][j] = (bits[i] == bits[j]) && pal[i + 1][j - 1];
        }
    }

    /* dynamic programming: dp[i] = number of palindromic partitions
       of the first i bits (prefix of length i).  dp[0] represents the
       empty prefix. */
    dp[0] = (seq_t)1;
    for (size_t i = 1; i <= len; ++i) {
        seq_t sum = 0;
        for (size_t j = 0; j < i; ++j) {
            if (pal[j][i - 1]) {
                sum += dp[j];
            }
        }
        dp[i] = sum;
    }

    return dp[len];
}
