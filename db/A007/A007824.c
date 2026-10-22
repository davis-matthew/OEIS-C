/*
 * A007824  a(n) = f(a(n-1)), with f(m) = Sum i*b(i)*2^(i-1), m = Sum
 *          b(i)*2^i, and starting value 16.
 * https://oeis.org/A007824
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = f(a(n-1)), a(0) = 16, where f(m) = Sum i*b(i)*2^(i-1) for
 *            m = Sum b(i)*2^i; f is computed by splitting m = hi*2^h + lo:
 *            f(m) = 2^h*f(hi) + h*2^(h-1)*hi + f(lo)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 18, u128 n <= 19,
 *            big any n
 * Helpers:   grow_zero, seq_is_fixed_width, u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/grow_zero.h"
#include "../utils/u128_to_seq.h"
#include "../utils/seq_is_fixed_width.h"

static seq_t deriv(seq_t m, idx_t nb) {
    if (m == 0) return (seq_t)0;
    if (nb <= 32) {
        uint64_t c = (uint64_t)(long long)(m & 0xFFFFFFFFu);
        uint64_t r = 0;
        idx_t i;
        for (i = 1; i < 32; i++)
            if ((c >> i) & 1u) r += (uint64_t)i << (i - 1);
        return u128_to_seq((unsigned __int128)r);
    }
    idx_t h = nb / 2;
    seq_t hi = m >> h;
    seq_t lo = m - (hi << h);
    seq_t res = deriv(hi, nb - h) << h;
    res += ((seq_t)h * hi) << (h - 1);
    res += deriv(lo, h);
    return res;
}

static seq_t f_step(seq_t m) {
    idx_t nb;
    if (seq_is_fixed_width()) {
        nb = (idx_t)(8 * sizeof(seq_t));
    } else {
        nb = 32;
        while ((m >> nb) != 0) nb *= 2;
    }
    return deriv(m, nb);
}

static seq_t *memo = 0;
static idx_t memo_len = 0;

seq_t a(idx_t n) {
    if (n < memo_len) return memo[n];
    idx_t newlen = memo_len ? memo_len : 16;
    while (newlen <= n) newlen *= 2;
    memo = grow_zero(memo, memo_len, newlen);
    idx_t start = memo_len;
    if (start == 0) {
        memo[0] = 16;
        start = 1;
    }
    idx_t i;
    for (i = start; i <= n; i++)
        memo[i] = f_step(memo[i - 1]);
    memo_len = n + 1;
    return memo[n];
}
