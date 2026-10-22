/*
 * A006973  Dimensions of representations by Witt vectors.
 * https://oeis.org/A006973
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   log of Product_{k>=1} (1 + a(k)*x^k/k!) = -x - log(1-x) gives
 *            a(n) = (n-1)! + Sum_{k|n, 1<k<n} (-1)^(n/k) *
 *            (n-1)!/((k-1)!*(k!)^(n/k-1)) * a(k)^(n/k), a(1) = 0
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 35,
 *            big any n
 * Helpers:   grow_zero, seq_is_fixed_width, u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/grow_zero.h"
#include "../utils/u128_to_seq.h"
#include "../utils/seq_is_fixed_width.h"

static seq_t *memo = NULL;
static bool *have = NULL;
static seq_t *fact = NULL;
static idx_t cap = 0;

static void ensure(idx_t n) {
    if (n < cap) return;
    idx_t newcap = n + 1;
    memo = grow_zero(memo, cap, newcap);
    fact = grow_zero(fact, cap, newcap);
    have = (bool *)realloc(have, newcap * sizeof(bool));
    for (idx_t i = cap; i < newcap; i++) {
        have[i] = false;
        if (i == 0) fact[0] = 1;
        else fact[i] = fact[i - 1] * (seq_t)i;
    }
    cap = newcap;
}

static seq_t spow(seq_t b, idx_t e) {
    seq_t r = 1;
    while (e) {
        if (e & 1) r = r * b;
        e >>= 1;
        if (e) b = b * b;
    }
    return r;
}

/* binomial in 128-bit native arithmetic, reduced into seq_t (fixed-width builds) */
static seq_t small_binom(idx_t m, idx_t r) {
    unsigned __int128 v = 1;
    for (idx_t i = 0; i < r; i++)
        v = v * (unsigned __int128)(m - i) / (unsigned __int128)(i + 1);
    return u128_to_seq(v);
}

seq_t a(idx_t n) {
    if (n <= 1) return 0;
    ensure(n);
    if (have[n]) return memo[n];
    bool fixed = seq_is_fixed_width();
    seq_t pos = 0, neg = 0;
    for (idx_t k = 2; 2 * k <= n; k++) {
        if (n % k != 0) continue;
        idx_t d = n / k;
        seq_t ak = a(k);
        seq_t c;
        if (fixed) {
            c = small_binom(n - 1, k - 1);
            for (idx_t j = 1; j < d; j++) c = c * small_binom(j * k, k);
        } else {
            c = fact[n - 1] / (fact[k - 1] * spow(fact[k], d - 1));
        }
        seq_t t = c * spow(ak, d);
        if (d & 1) neg += t;
        else pos += t;
    }
    seq_t r = fact[n - 1] + pos - neg;
    memo[n] = r;
    have[n] = true;
    return r;
}
