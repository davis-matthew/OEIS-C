/*
 * A064696  Smallest value of s>0 such that A064683(s) is divisible by n, or 0
 *          if n belongs to A064695.
 * https://oeis.org/A064696
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least s>0 such that N(s) is divisible by N, else 0, where
 *            N = n+1 and N(s) is N with s zeros inserted between its digits;
 *            computed by a search over s
 * Range:     u8 n <= 55, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Validated: partial, pass, 66 terms through n = 65 (2026-11-07)
 * Helpers:   mulmod_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mulmod_seq.h"

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n + 1;
    uint8_t digits[64];
    unsigned int len = 0;
    seq_t tmp = N;
    while (tmp > (seq_t)0) {
        digits[len++] = (uint8_t)(long long)(tmp % (seq_t)10);
        tmp /= (seq_t)10;
    }
    for (unsigned int i = 0; i < len / 2; ++i) {
        uint8_t t = digits[i];
        digits[i] = digits[len - 1 - i];
        digits[len - 1 - i] = t;
    }

    seq_t pw = (seq_t)10 % N;
    seq_t limit = N + (seq_t)200;
    for (seq_t s = 1; s <= limit; ++s) {
        pw = mulmod_seq(pw, (seq_t)10, N);
        seq_t val = (seq_t)0;
        for (unsigned int i = 0; i < len; ++i) {
            val = mulmod_seq(val, pw, N);
            val = (val + (seq_t)digits[i]) % N;
        }
        if (val == (seq_t)0) return s;
    }
    return (seq_t)0;
}
