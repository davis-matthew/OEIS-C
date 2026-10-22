/*
 * A005479  Prime Lucas numbers (cf. A000032).
 * https://oeis.org/A005479
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, changed
 * Formula:   search over Lucas numbers L(k) (L(0)=2, L(1)=1,
 *            L(k)=L(k-1)+L(k-2)); only k prime or a power of 2 can give
 *            primes; a(n) = n-th prime L(k), tested by trial division plus
 *            Miller-Rabin
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 14, u64 n <= 19, u128 n <= 20,
 *            big any n
 * Helpers:   seq_is_big, small_index_ok
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/seq_is_big.h"
#include "../utils/small_index_ok.h"

static uint64_t *idx_list = NULL;
static idx_t idx_count = 0;
static idx_t idx_cap = 0;
static uint64_t scan_k = 0;
static seq_t scan_L0 = 2;   /* L(scan_k) */
static seq_t scan_L1 = 1;   /* L(scan_k + 1) */

static seq_t mulmod(seq_t a, seq_t b, seq_t m) {
    seq_t r;
    if (seq_is_big()) return a * b % m;
    r = 0;
    a %= m;
    while (b > 0) {
        if ((b & 1) != 0) {
            if (r >= m - a) r = r - (m - a);
            else r = r + a;
        }
        b >>= 1;
        if (b > 0) {
            if (a >= m - a) a = a - (m - a);
            else a = a + a;
        }
    }
    return r;
}

static seq_t powmod(seq_t b, seq_t e, seq_t m) {
    seq_t r = 1;
    b %= m;
    while (e > 0) {
        if ((e & 1) != 0) r = mulmod(r, b, m);
        e >>= 1;
        if (e > 0) b = mulmod(b, b, m);
    }
    return r;
}

static bool is_prime_seq(seq_t m) {
    static const int small_p[12] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
    seq_t d, x, m1;
    int s, i, j;
    if (m < 2) return false;
    for (i = 0; i < 12; i++) {
        if (m == (seq_t)small_p[i]) return true;
        if (m % small_p[i] == 0) return false;
    }
    if (m < 41 * 41) return true;
    m1 = m - 1;
    d = m1;
    s = 0;
    while ((d & 1) == 0) { d >>= 1; s++; }
    for (i = 0; i < 12; i++) {
        bool comp = true;
        x = powmod((seq_t)small_p[i], d, m);
        if (x == 1 || x == m1) continue;
        for (j = 1; j < s; j++) {
            x = mulmod(x, x, m);
            if (x == m1) { comp = false; break; }
        }
        if (comp) return false;
    }
    return true;
}

static void push_index(uint64_t k) {
    if (idx_count >= idx_cap) {
        idx_t nc = idx_cap ? idx_cap * 2 : 64;
        idx_list = (uint64_t *)realloc(idx_list, (size_t)nc * sizeof(uint64_t));
        idx_cap = nc;
    }
    idx_list[idx_count++] = k;
}

seq_t a(idx_t n) {
    uint64_t k, target, i;
    seq_t x, y, t;
    while (idx_count < n) {
        if (small_index_ok(scan_k) && is_prime_seq(scan_L0))
            push_index(scan_k);
        t = scan_L0 + scan_L1;
        scan_L0 = scan_L1;
        scan_L1 = t;
        scan_k++;
    }
    target = idx_list[n - 1];
    x = 2;
    y = 1;
    for (k = 0; k < target; k++) {
        t = x + y;
        x = y;
        y = t;
    }
    (void)i;
    return x;
}
