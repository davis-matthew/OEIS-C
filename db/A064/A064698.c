/*
 * A064698  Number of values of s from 1 to n such that A064683(s) is
 *          divisible by s.
 * https://oeis.org/A064698
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{ s : 1 <= s <= n, n(s) == 0 (mod n) }, where n(s) is n
 *            with s zeros inserted between consecutive decimal digits; search
 *            over s
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Validated: partial, pass, 74 terms through n = 74 (2026-11-07)
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
    seq_t m = (seq_t)n;

    /* decimal digits of m, most significant first */
    uint8_t digits[64];
    unsigned int len = 0;
    idx_t tmp = n;
    while (tmp > 0) {
        digits[len++] = (uint8_t)(tmp % 10);
        tmp /= 10;
    }
    for (unsigned int i = 0; i < len / 2; ++i) {
        uint8_t t = digits[i];
        digits[i] = digits[len - 1 - i];
        digits[len - 1 - i] = t;
    }

    seq_t cnt = 0;
    seq_t x = (seq_t)10 % m;            /* becomes 10^(s+1) mod m */
    for (idx_t s = 1; s <= n; ++s) {
        x = mulmod_seq(x, (seq_t)10, m);
        seq_t val = 0;
        for (unsigned int i = 0; i < len; ++i) {
            val = mulmod_seq(val, x, m);
            val = (val + (seq_t)digits[i]) % m;
        }
        if (val == (seq_t)0) ++cnt;
    }
    return cnt;
}
