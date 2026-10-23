/*
 * A045155  Numbers whose base-4 representation contains exactly four 2's and
 *          two 3's.
 * https://oeis.org/A045155
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation method: enumerates base‑4 numbers that contain exactly four 2's
   and two 3's, selects the n‑th smallest in decimal form. */
#include <stdint.h>
#include <stddef.h>

static idx_t binom(idx_t n, idx_t k) {
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    idx_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (n - k + i) / i;
    }
    return res;
}

static idx_t count_len(idx_t L) {
    if (L < 6) return 0;
    idx_t part1 = binom(L, 4);
    idx_t part2 = binom(L - 4, 2);
    idx_t exp   = L - 6;
    idx_t part3 = exp < 64 ? (1ULL << exp) : 0;
    return part1 * part2 * part3;
}

static seq_t generate_value(idx_t L, idx_t k) {
    idx_t twos   = 4;
    idx_t threes = 2;
    seq_t value  = 0;
    for (idx_t pos = 0; pos < L; ++pos) {
        idx_t rem = L - pos - 1;
        for (int d = 0; d <= 3; ++d) {
            if (pos == 0 && d == 0) continue;
            if (d == 2 && twos == 0) continue;
            if (d == 3 && threes == 0) continue;
            idx_t t2   = twos   - (d == 2 ? 1 : 0);
            idx_t t3   = threes - (d == 3 ? 1 : 0);
            idx_t free = rem - t2 - t3;
            if (free < 0) continue;
            idx_t cnt = binom(rem, t2);
            cnt *= binom(rem - t2, t3);
            cnt *= (free < 64 ? (1ULL << free) : 0);
            if (k <= cnt) {
                value = value * (seq_t)4 + (seq_t)d;
                if (d == 2) --twos;
                if (d == 3) --threes;
                break;
            } else {
                k -= cnt;
            }
        }
    }
    return value;
}

seq_t a(idx_t n) {
    idx_t remaining = n;
    idx_t L = 6;
    while (1) {
        idx_t cnt = count_len(L);
        if (remaining <= cnt) {
            return generate_value(L, remaining);
        }
        remaining -= cnt;
        ++L;
    }
}
