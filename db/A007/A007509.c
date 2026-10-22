/*
 * A007509  Numerator of Sum_{k=0..n} (-1)^k/(2*k+1).
 * https://oeis.org/A007509
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice, frac
 * Formula:   a(n) = numerator(Sum_{k=0..n} (-1)^k/(2*k+1)); computed as the
 *            running reduced fraction S(k) = S(k-1) + (-1)^k/(2k+1), S(0) = 1
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 23, u128 n <= 47,
 *            big any n
 * Helpers:   gcd_seq, grow_zero, seq_is_fixed_width, u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd_seq.h"
#include "../utils/grow_zero.h"
#include "../utils/seq_is_fixed_width.h"
#include "../utils/u128_to_seq.h"

static unsigned __int128 g128(unsigned __int128 x, unsigned __int128 y) {
    while (y != 0) {
        unsigned __int128 t = x % y;
        x = y;
        y = t;
    }
    return x;
}

static seq_t a_fixed(idx_t n) {
    unsigned __int128 num = 1, den = 1;
    for (uint64_t k = 1; k <= n; k++) {
        unsigned __int128 q = 2 * (unsigned __int128)k + 1;
        unsigned __int128 g = g128(den, q);
        unsigned __int128 dg = den / g, qg = q / g;
        unsigned __int128 nn = (k % 2 == 0) ? num * qg + dg : num * qg - dg;
        unsigned __int128 L = dg * q;
        unsigned __int128 h = g128(nn, L);
        num = nn / h;
        den = L / h;
    }
    return u128_to_seq(num);
}

static seq_t *memo = 0;
static idx_t memo_cap = 0;
static idx_t memo_len = 0;
static seq_t cur_num;
static seq_t cur_den;

seq_t a(idx_t n) {
    if (seq_is_fixed_width()) return a_fixed(n);
    if (memo_len == 0) {
        memo_cap = 64;
        memo = grow_zero(memo, 0, memo_cap);
        cur_num = 1;
        cur_den = 1;
        memo[0] = cur_num;
        memo_len = 1;
    }
    while (memo_len <= n) {
        uint64_t k = memo_len;
        seq_t q = (seq_t)(2 * k + 1);
        seq_t g = gcd_seq(cur_den, q);
        seq_t dg = cur_den / g;
        seq_t qg = q / g;
        seq_t nn = cur_num * qg;
        if (k % 2 == 0) nn += dg; else nn -= dg;
        seq_t L = dg * q;
        seq_t h = gcd_seq(nn, L);
        cur_num = nn / h;
        cur_den = L / h;
        if (memo_len >= memo_cap) {
            idx_t nc = memo_cap * 2;
            memo = grow_zero(memo, memo_cap, nc);
            memo_cap = nc;
        }
        memo[memo_len] = cur_num;
        memo_len++;
    }
    return memo[n];
}
