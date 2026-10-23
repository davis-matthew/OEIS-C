/*
 * A045020  Numbers whose base-4 representation contains exactly one 0 and two
 *          1's.
 * https://oeis.org/A045020
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 6660, u32 n <= 9000, u64 n <= 9000,
 *            u128 n <= 9000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers whose base-4 representation contains exactly one 0 and two 1's. */
#include <stdint.h>

/* Compute binomial coefficient C(n,k) using 128-bit arithmetic to avoid overflow. */
static uint64_t binom_uint64(uint64_t n, uint64_t k) {
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    uint64_t res = 1;
    for (uint64_t i = 1; i <= k; ++i) {
        __uint128_t temp = res;
        temp = temp * (n - k + i);
        temp /= i;
        res = (uint64_t)temp;
    }
    return res;
}

/* 2^exp for exp < 64. */
static uint64_t pow2_uint64(uint64_t exp) {
    return 1ULL << exp;
}

/* Count of numbers of length L (base-4 digits) that satisfy the pattern. */
static uint64_t count_for_length(uint64_t L) {
    if (L < 3) return 0;
    uint64_t pow2 = pow2_uint64(L - 3);
    uint64_t ways = (L - 1) * binom_uint64(L - 1, 2) * pow2;
    return ways;
}

/* Count of completions for remaining positions. */
static uint64_t count_remaining(uint64_t rem, uint64_t r0, uint64_t r1) {
    if (r0 > rem || r1 > rem - r0) return 0;
    uint64_t c1 = binom_uint64(rem, r0);
    uint64_t c2 = binom_uint64(rem - r0, r1);
    uint64_t pow2 = pow2_uint64(rem - r0 - r1);
    return c1 * c2 * pow2;
}

/* External linkage as required. */
seq_t a(idx_t n) {
    /* Find the length L that contains the nth term. */
    uint64_t L = 3;
    uint64_t cumulative = 0;
    while (1) {
        uint64_t ways = count_for_length(L);
        cumulative += ways;
        if (cumulative >= n)
            break;
        ++L;
    }

    /* Offset within numbers of length L. */
    uint64_t offset = n - (cumulative - count_for_length(L)) - 1;  /* 0‑based */

    /* Build the number digit by digit. */
    uint64_t rem_zero = 1, rem_one = 2;
    seq_t value = 0;

    for (uint64_t pos = 0; pos < L; ++pos) {
        for (int d = 0; d <= 3; ++d) {
            if (pos == 0 && d == 0) continue;
            if (d == 0 && rem_zero == 0) continue;
            if (d == 1 && rem_one == 0) continue;

            uint64_t nz = rem_zero;
            uint64_t no = rem_one;
            if (d == 0) --nz;
            if (d == 1) --no;
            uint64_t rem_positions = L - pos - 1;
            uint64_t cnt_next = count_remaining(rem_positions, nz, no);

            if (offset < cnt_next) {
                /* Choose this digit and continue. */
                value = value * (seq_t)4 + (seq_t)d;
                rem_zero = nz;
                rem_one = no;
                break;
            } else {
                offset -= cnt_next;
            }
        }
    }

    return value;
}
