/*
 * A045292  Numbers whose base-5 representation contains exactly three 2's and
 *          three 4's.
 * https://oeis.org/A045292
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = nth number whose base‑5 representation contains exactly
 *            three 2's and three 4's (offset 1)
 * Range:     u8 none, u16 n <= 32, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t pow3_exp(uint32_t e) {
    uint64_t r = 1;
    while (e--) r *= 3;
    return r;
}

static uint64_t binom_uint64(uint64_t n, uint64_t k) {
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    __uint128_t res = 1;
    for (uint64_t i = 1; i <= k; ++i) {
        res = res * (n - k + i) / i;
    }
    return (uint64_t)res;
}

static uint64_t count_seq(uint64_t remLen, uint64_t remTwo, uint64_t remFour) {
    if (remTwo > remLen || remFour > remLen - remTwo) return 0;
    uint64_t remOthers = remLen - remTwo - remFour;
    uint64_t part = binom_uint64(remLen, remTwo);
    part = (uint64_t)((__uint128_t)part * binom_uint64(remLen - remTwo, remFour));
    __uint128_t prod = (__uint128_t)part * pow3_exp((uint32_t)remOthers);
    return (uint64_t)prod;
}

static uint64_t count_valid_len(uint64_t L) {
    if (L < 6) return 0;
    uint64_t total = count_seq(L, 3, 3);
    if (L >= 7) {
        total -= count_seq(L - 1, 3, 3); /* remove leading‑zero cases */
    }
    return total;
}

seq_t a(idx_t n) {
    if (n == 0) return 0; /* undefined for offset 1, but guard */
    uint64_t cumulative = 0;
    uint64_t L = 6;
    while (1) {
        uint64_t cnt = count_valid_len(L);
        if (cumulative + cnt >= n) break;
        cumulative += cnt;
        ++L;
    }
    uint64_t rank = (uint64_t)n - cumulative - 1; /* zero‑based rank within length L */

    seq_t value = 0;
    uint64_t remLen = L;
    uint64_t remTwo = 3;
    uint64_t remFour = 3;

    for (uint64_t pos = 0; pos < L; ++pos) {
        for (int d = 0; d <= 4; ++d) {
            if (pos == 0 && d == 0) continue;
            if (d == 2 && remTwo == 0) continue;
            if (d == 4 && remFour == 0) continue;

            uint64_t newRemTwo = remTwo;
            uint64_t newRemFour = remFour;
            if (d == 2) --newRemTwo;
            if (d == 4) --newRemFour;

            uint64_t cnt = count_seq(remLen - 1, newRemTwo, newRemFour);
            if (rank < cnt) {
                remLen--;
                remTwo = newRemTwo;
                remFour = newRemFour;
                value = (value * (seq_t)5) + (seq_t)d;
                break;
            } else {
                rank -= cnt;
            }
        }
    }
    return value;
}
