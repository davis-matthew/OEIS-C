/*
 * A232542  Primes in A100683.
 * https://oeis.org/A232542
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search -- the terms of A100683 (b(0) = -1, b(1) = 2, b(2) = 2,
 *            b(n) = b(n-1) + b(n-2) + b(n-3)) from b(1) on, kept when prime,
 *            in order
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Validated: partial, pass, 12 terms through n = 12 (2026-11-07)
 * Helpers:   is_prime_seq, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"
#include "../utils/is_prime_seq.h"

static seq_t *A232542_pr = NULL;
static idx_t A232542_cnt = 0;
static idx_t A232542_cap = 0;
static bool A232542_init = false;
static bool A232542_done = false;   /* fixed-width overflow reached */
static bool A232542_big = false;    /* arbitrary-precision seq_t */
static seq_t A232542_u, A232542_v, A232542_w;

static const unsigned A232542_small[] = {
    2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,79,83,89,97,
    101,103,107,109,113,127,131,137,139,149,151,157,163,167,173,179,181,191,
    193,197,199,211,223,227,229,233,239,241,251
};

static seq_t A232542_powmod(seq_t b, seq_t e, seq_t m)
{
    seq_t r = 1;
    b %= m;
    while (e > 0) {
        if ((e & 1) != 0) r = r * b % m;
        e >>= 1;
        if (e > 0) b = b * b % m;
    }
    return r;
}

/* Miller-Rabin for arbitrary-precision values (products do not overflow) */
static bool A232542_is_prime_big(seq_t x)
{
    if (x < 2) return false;
    size_t ns = sizeof(A232542_small) / sizeof(A232542_small[0]);
    for (size_t i = 0; i < ns; i++) {
        seq_t p = (seq_t)A232542_small[i];
        if (x == p) return true;
        if (x % p == 0) return false;
    }
    seq_t xm1 = x - 1;
    seq_t d = xm1;
    int s = 0;
    while (d % 2 == 0) { d /= 2; s++; }
    for (int i = 0; i < 20; i++) {
        seq_t y = A232542_powmod((seq_t)A232542_small[i], d, x);
        if (y == 1 || y == xm1) continue;
        bool comp = true;
        for (int r = 1; r < s; r++) {
            y = y * y % x;
            if (y == xm1) { comp = false; break; }
            if (y == 1) break;
        }
        if (comp) return false;
    }
    return true;
}

static bool A232542_isp(seq_t x)
{
    if (A232542_big) return A232542_is_prime_big(x);
    return is_prime_seq(x);
}

static bool A232542_push(seq_t x)
{
    if (A232542_cnt >= A232542_cap) {
        idx_t nc = A232542_cap ? A232542_cap * 2 : 64;
        seq_t *t = mem_resize(seq_t, A232542_pr, nc);
        if (!t) return false;
        A232542_pr = t;
        A232542_cap = nc;
    }
    A232542_pr[A232542_cnt++] = x;
    return true;
}

seq_t a(idx_t n)
{
    if (!A232542_init) {
        A232542_init = true;
        /* detect arbitrary precision: 2^256 wraps to 0 in any fixed width */
        seq_t t = 2;
        for (int i = 0; i < 8; i++) t = t * t;
        A232542_big = (t != 0);
        A232542_u = 2; A232542_v = 2; A232542_w = 3; /* b(1), b(2), b(3) */
    }
    while (A232542_cnt < n && !A232542_done) {
        seq_t x = A232542_u;
        if (A232542_isp(x)) {
            if (!A232542_push(x)) return 0;
        }
        seq_t s = A232542_u + A232542_v;
        seq_t nx = s + A232542_w;
        if (!A232542_big && (s < A232542_u || nx < s)) {
            /* next term overflows; v and w are still valid terms */
            A232542_done = true;
            if (A232542_isp(A232542_v) && !A232542_push(A232542_v)) return 0;
            if (A232542_isp(A232542_w) && !A232542_push(A232542_w)) return 0;
            break;
        }
        A232542_u = A232542_v;
        A232542_v = A232542_w;
        A232542_w = nx;
    }
    if (n >= 1 && n <= A232542_cnt) return A232542_pr[n - 1];
    return 0;
}
