/*
 * A103642  Number of tilings of the Aztec dungeon E_n.
 * https://oeis.org/A103642
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = init[n mod 6] * 13^(12*⌊n/6⌋*(⌊n/6⌋-1)+⌊n/6⌋*(4*(n mod
 *            6)+16)), where init = {1, 26, 2197, 371293, 1631461442,
 *            23298085122481}
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_uint.h"

static const seq_t init[6] = {
    (seq_t)1,
    (seq_t)26,
    (seq_t)2197,
    (seq_t)371293,
    (seq_t)1631461442,
    (seq_t)23298085122481ULL
};

seq_t a(idx_t n) {
    unsigned long long k = n / 6;
    unsigned long long r = n % 6;
    unsigned long long exp;

    if (k == 0) {
        exp = 0;
    } else {
        exp = 12ULL * k * (k - 1ULL) + k * (4ULL * r + 16ULL);
    }

    seq_t pow13 = pow_uint((seq_t)13, exp);
    return pow13 * init[r];
}
