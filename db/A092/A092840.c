/*
 * A092840  Primes in A001003.
 * https://oeis.org/A092840
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search: compute b(k) = A001003(k) by (k+1)*b(k) = (6k-3)*b(k-1)
 *            - (k-2)*b(k-2), b(0) = b(1) = 1, and keep the terms that are
 *            prime (trial division + Miller-Rabin)
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Validated: standard, pass, 5 terms through n = 5 (2026-11-07)
 * Helpers:   is_prime_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_seq.h"

#define A092840_MAXFOUND 16
#define A092840_MAXIDX 6000
#define A092840_TDLIM 2000

static seq_t A092840_found[A092840_MAXFOUND];
static idx_t A092840_cnt = 0;
static idx_t A092840_i = 1;
static bool A092840_init = false;
static bool A092840_big = false;
static bool A092840_done = false;
static seq_t A092840_x1;
static seq_t A092840_x2;
static unsigned A092840_sp[400];
static int A092840_nsp = 0;

static void A092840_step(void) {
    idx_t i = A092840_i + 1;
    idx_t d = i + 1;
    seq_t q1 = A092840_x1 / (seq_t)d;
    seq_t q2 = A092840_x2 / (seq_t)d;
    long long r1 = (long long)(A092840_x1 % (seq_t)d);
    long long r2 = (long long)(A092840_x2 % (seq_t)d);
    long long t = (long long)(6 * i - 3) * r1 - (long long)(i - 2) * r2;
    long long tq = t / (long long)d; /* exact */
    seq_t v = q1 * (seq_t)(6 * i - 3);
    if (tq > 0) v += (seq_t)(unsigned long long)tq;
    v -= q2 * (seq_t)(i - 2);
    if (tq < 0) v -= (seq_t)(unsigned long long)(-tq);
    A092840_x2 = A092840_x1;
    A092840_x1 = v;
    A092840_i = i;
}

static seq_t A092840_powmod(seq_t b, seq_t e, seq_t m) {
    seq_t r = 1;
    b %= m;
    while (e > 0) {
        if ((e & 1) != 0) r = (r * b) % m;
        e >>= 1;
        if (e > 0) b = (b * b) % m;
    }
    return r;
}

static bool A092840_mr(seq_t m, unsigned base) {
    seq_t m1 = m - 1;
    seq_t d = m1;
    unsigned s = 0, j;
    seq_t x;
    while ((d & 1) == 0) { d >>= 1; s++; }
    x = A092840_powmod((seq_t)base, d, m);
    if (x == 1 || x == m1) return true;
    for (j = 1; j < s; j++) {
        x = (x * x) % m;
        if (x == m1) return true;
        if (x == 1) return false;
    }
    return false;
}

/* bignum-only primality test */
static bool A092840_isprime_big(seq_t x) {
    int k;
    static const unsigned bases[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
    if (x < 2) return false;
    for (k = 0; k < A092840_nsp; k++) {
        unsigned p = A092840_sp[k];
        if (x == (seq_t)p) return true;
        if (x % (seq_t)p == 0) return false;
    }
    if (x < (seq_t)A092840_TDLIM * (seq_t)A092840_TDLIM) return true;
    for (k = 0; k < 12; k++)
        if (!A092840_mr(x, bases[k])) return false;
    return true;
}

seq_t a(idx_t n) {
    if (!A092840_init) {
        seq_t t = 1;
        int k;
        unsigned p;
        for (k = 0; k < 8; k++) t *= (seq_t)4294967296ULL;
        A092840_big = (t != 0);
        for (p = 2; p < A092840_TDLIM; p++) {
            unsigned q;
            bool pr = true;
            for (q = 2; q * q <= p; q++)
                if (p % q == 0) { pr = false; break; }
            if (pr) A092840_sp[A092840_nsp++] = p;
        }
        A092840_x1 = 1; /* b(1) */
        A092840_x2 = 1; /* b(0) */
        A092840_i = 1;
        A092840_init = true;
    }
    while (!A092840_done && A092840_cnt < n && A092840_cnt < A092840_MAXFOUND &&
           A092840_i < A092840_MAXIDX) {
        seq_t prev = A092840_x1;
        bool pr;
        A092840_step();
        if (!A092840_big && A092840_i > 2 && A092840_x1 <= prev) {
            A092840_done = true; /* overflow in fixed-width build */
            break;
        }
        if (A092840_big) pr = A092840_isprime_big(A092840_x1);
        else pr = is_prime_seq(A092840_x1);
        if (pr) A092840_found[A092840_cnt++] = A092840_x1;
    }
    if (n >= 1 && n <= A092840_cnt) return A092840_found[n - 1];
    return 0;
}
