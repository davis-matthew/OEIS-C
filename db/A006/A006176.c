/*
 * A006176  Witt vector *5!.
 * https://oeis.org/A006176
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/n)*((5n)!/(n!)^5 - Sum_{d|n, d<n} d*a(d)^(n/d)), i.e.
 *            Sum_{d|n} d*a(d)^(n/d) = (5n)!/(n!)^5 = A008978(n) (ghost
 *            components); a(1) = 120
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 6, u128 n <= 10,
 *            big any n
 * Helpers:   a000248_pow, fx, grow_zero, multinom5, seq_is_fixed_width,
 *            u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/grow_zero.h"
#include "../utils/a000248_pow.h"
#include "../utils/seq_is_fixed_width.h"
#include "../utils/u128_to_seq.h"
#include "../utils/fx.h"
#include "../utils/multinom5.h"

static seq_t *memo = NULL;
static idx_t cap = 0;

static seq_t big_a(idx_t n)
{
    idx_t d;
    seq_t s;
    if (n >= cap) {
        idx_t nc = cap ? cap : 16;
        while (nc <= n) nc *= 2;
        memo = grow_zero(memo, cap, nc);
        cap = nc;
    }
    if (memo[n] != 0) return memo[n];
    s = multinom5(n);
    for (d = 1; d < n; d++)
        if (n % d == 0)
            s -= (seq_t)d * a000248_pow(big_a(d), n / d);
    s /= n;
    memo[n] = s;
    return s;
}

seq_t a(idx_t n)
{
    if (seq_is_fixed_width()) {
        if (n > 12) return 0;
        return u128_to_seq(fx(n));
    }
    return big_a(n);
}
