/*
 * A005179  Smallest number with exactly n divisors.
 * https://oeis.org/A005179
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   search over all factorizations n = d_1*d_2*...*d_k with d_1 >=
 *            d_2 >= ... >= 2; a(n) = min of prod p_i^(d_i - 1), p_i the i-th
 *            prime (with saturating arithmetic for fixed-width seq_t)
 * Range:     u8 n <= 10, u16 n <= 16, u32 n <= 36, u64 n <= 66, u128 n <= 130,
 *            big any n
 * Helpers:   seq_is_fixed_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/seq_is_fixed_width.h"

static const uint64_t a5179_primes[] = {
    2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71,
    73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127, 131, 137, 139, 149, 151,
    157, 163, 167, 173, 179, 181, 191, 193, 197, 199, 211, 223, 227, 229, 233,
    239, 241, 251, 257, 263, 269, 271, 277, 281, 283, 293, 307, 311, 313, 317
};

static bool a5179_fixed;
static seq_t a5179_max;

static seq_t a5179_mul(seq_t x, seq_t y) {
    if (a5179_fixed) {
        if (x == 0 || y == 0) return 0;
        if (x == a5179_max || y == a5179_max) return a5179_max;
        if (y > a5179_max / x) return a5179_max;
    }
    return x * y;
}

static seq_t a5179_pow(seq_t b, idx_t e) {
    seq_t r = 1;
    while (e > 0) {
        if (e & 1) r = a5179_mul(r, b);
        e >>= 1;
        if (e > 0) b = a5179_mul(b, b);
    }
    return r;
}

static void a5179_search(idx_t n, int i, idx_t maxd, seq_t cur,
                         seq_t *best, bool *found) {
    if (n == 1) {
        if (!*found || cur < *best) {
            *best = cur;
            *found = true;
        }
        return;
    }
    idx_t lim = n < maxd ? n : maxd;
    for (idx_t d = 2; d <= lim; d++) {
        if (n % d != 0) continue;
        seq_t v = a5179_mul(cur, a5179_pow((seq_t)a5179_primes[i], d - 1));
        if (*found && v >= *best) continue;
        a5179_search(n / d, i + 1, d, v, best, found);
    }
}

seq_t a(idx_t n) {
    if (n <= 1) return 1;
    a5179_fixed = seq_is_fixed_width();
    a5179_max = 0;
    if (a5179_fixed) {
        a5179_max = 0;
        a5179_max = ~a5179_max;
    }
    seq_t best = 0;
    bool found = false;
    a5179_search(n, 0, n, (seq_t)1, &best, &found);
    return best;
}
