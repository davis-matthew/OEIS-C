/*
 * A215246  a(n) = maximal value of k such that A215244(k) = n.
 * https://oeis.org/A215246
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = max{k : A215244(k) = n}, where A215244(k) is the number
 *            of ways to write the binary expansion of k as a concatenation of
 *            palindromes. The code searches by bit length. It stops after the
 *            first length block in which every k has A215244(k) > n
 * Range:     u8 n <= 19, u16 n <= 307, u32 n <= 307, u64 n <= 307,
 *            u128 n <= 307, big any n
 * Validated: standard, pass, 73 terms through n = 294 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

static uint64_t A215246_cnt(uint64_t n)
{
    bool pal[64][64];
    unsigned char bits[64];
    uint64_t dp[65];
    size_t len = 0;
    uint64_t t = n;
    while (t) { ++len; t >>= 1; }
    for (size_t i = 0; i < len; ++i)
        bits[i] = (unsigned char)((n >> (len - 1 - i)) & 1);
    for (size_t i = 0; i < len; ++i) {
        pal[i][i] = true;
        if (i + 1 < len) pal[i][i + 1] = (bits[i] == bits[i + 1]);
    }
    for (size_t L = 3; L <= len; ++L)
        for (size_t i = 0; i + L <= len; ++i) {
            size_t j = i + L - 1;
            pal[i][j] = (bits[i] == bits[j]) && pal[i + 1][j - 1];
        }
    dp[0] = 1;
    for (size_t i = 1; i <= len; ++i) {
        uint64_t s = 0;
        for (size_t j = 0; j < i; ++j)
            if (pal[j][i - 1]) s += dp[j];
        dp[i] = s;
    }
    return dp[len];
}

seq_t a(idx_t n)
{
    uint64_t best = 0;
    for (int L = 1; L <= 62; ++L) {
        uint64_t lo = (uint64_t)1 << (L - 1);
        uint64_t hi = (uint64_t)1 << L;
        uint64_t minc = ~(uint64_t)0;
        for (uint64_t k = lo; k < hi; ++k) {
            uint64_t c = A215246_cnt(k);
            if (c == (uint64_t)n) best = k;
            if (c < minc) minc = c;
        }
        if (minc > (uint64_t)n) break;
    }
    return (seq_t)best;
}
