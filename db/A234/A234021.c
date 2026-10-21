/*
 * A234021  Numbers with digits in nondecreasing order such that sum of
 *          squares of digits is a prime.
 * https://oeis.org/A234021
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = number with nondecreasing digits whose sum of squares of
 *            digits is prime. Computed by enumeration
 * Range:     u8 n <= 29, u16 n <= 413, u32 n <= 2091, u64 n <= 2091,
 *            u128 n <= 2091, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool is_prime_small(uint64_t x) {
    if (x < 2) return false;
    for (uint64_t i = 2; i * i <= x; ++i)
        if (x % i == 0) return false;
    return true;
}

static bool gen_len(idx_t len, int start, seq_t val, int sumsq,
                    idx_t *cnt, idx_t target, seq_t *result) {
    if (len == 0) {
        if (is_prime_small((uint64_t)sumsq)) {
            ++(*cnt);
            if (*cnt == target) {
                *result = val;
                return true;
            }
        }
        return false;
    }
    for (int d = start; d <= 9; ++d) {
        if (gen_len(len - 1, d, val * 10 + (seq_t)d,
                    sumsq + d * d, cnt, target, result))
            return true;
    }
    return false;
}

seq_t a(idx_t n) {
    idx_t target = n;
    idx_t count = 0;
    seq_t res = 0;
    for (idx_t len = 2; /* infinite loop until found */ ; ++len) {
        if (gen_len(len, 1, 0, 0, &count, target, &res))
            return res;
    }
}
