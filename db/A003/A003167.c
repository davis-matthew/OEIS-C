/*
 * A003167  Number of n-dimensional cuboids with integral edge lengths for
 *          which volume = surface area.
 * https://oeis.org/A003167
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, hard, more
 * Formula:   a(n) = number of nondecreasing n-tuples (x_1,...,x_n) with 1/x_1
 *            + ... + 1/x_n = 1/2. Computed by a bounded depth-first search:
 *            q/p < x_i <= k*q/p for the remaining fraction p/q with k terms
 *            left. The last two terms are counted by enumerating divisors of
 *            q^2 (or by a short direct scan)
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   a3167_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/a3167_gcd.h"

static uint64_t a3167_cnt;
static uint64_t a3167_pr[1024];
static int a3167_npr;

static void a3167_addprimes(uint64_t x) {
    uint64_t d;
    for (d = 2; d * d <= x; d++) {
        if (x % d == 0) {
            int i; bool found = false;
            for (i = 0; i < a3167_npr; i++) if (a3167_pr[i] == d) { found = true; break; }
            if (!found && a3167_npr < 1024) a3167_pr[a3167_npr++] = d;
            while (x % d == 0) x /= d;
        }
    }
    if (x > 1) {
        int i; bool found = false;
        for (i = 0; i < a3167_npr; i++) if (a3167_pr[i] == x) { found = true; break; }
        if (!found && a3167_npr < 1024) a3167_pr[a3167_npr++] = x;
    }
}

static void a3167_divgen(int i, uint64_t A, const uint64_t *fp, const int *fe, int nf,
                         uint64_t q, uint64_t p, uint64_t res, uint64_t Amin) {
    int e;
    uint64_t v = A;
    if (i == nf) {
        if (A >= Amin && A % p == res) a3167_cnt++;
        return;
    }
    for (e = 0; e <= 2 * fe[i]; e++) {
        a3167_divgen(i + 1, v, fp, fe, nf, q, p, res, Amin);
        if (e == 2 * fe[i] || v > q / fp[i]) break;
        v *= fp[i];
    }
}

/* count x <= y, x >= m with 1/x + 1/y = p/q (p/q reduced) */
static void a3167_two(uint64_t p, uint64_t q, uint64_t m) {
    uint64_t lo = q / p + 1, hi = 2 * q / p;
    uint64_t fp[1024];
    int fe[1024];
    int nf = 0, i;
    uint64_t r = q, d = 1;
    if (lo < m) lo = m;
    if (lo > hi) return;
    for (i = 0; i < a3167_npr && r > 1; i++) {
        uint64_t pp = a3167_pr[i];
        if (r % pp == 0) {
            int e = 0;
            while (r % pp == 0) { r /= pp; e++; }
            fp[nf] = pp; fe[nf] = e; nf++;
            if (d < ((uint64_t)1 << 40)) d *= (uint64_t)(2 * e + 1);
        }
    }
    if (r > 1) { /* should not happen; fall back to a plain scan */
        d = (uint64_t)-1;
    }
    if (hi - lo + 1 <= d / 2 + 1) {
        uint64_t x;
        for (x = lo; x <= hi; x++) {
            uint64_t num = p * x - q;
            unsigned __int128 prod = (unsigned __int128)q * x;
            if (prod % num == 0) a3167_cnt++;
        }
    } else {
        uint64_t res = (p - q % p) % p;
        uint64_t Amin = (p * m > q) ? p * m - q : 0;
        a3167_divgen(0, 1, fp, fe, nf, q, p, res, Amin);
    }
}

static void a3167_rec(uint64_t p, uint64_t q, uint64_t m, idx_t k) {
    uint64_t lo, hi, x;
    if (k == 1) {
        if (p == 1 && q >= m) a3167_cnt++;
        return;
    }
    if (k == 2) {
        a3167_two(p, q, m);
        return;
    }
    lo = q / p + 1;
    if (lo < m) lo = m;
    hi = (uint64_t)k * q / p;
    for (x = lo; x <= hi; x++) {
        uint64_t np = p * x - q;
        uint64_t nq = q * x;
        uint64_t g = a3167_gcd(np, nq);
        int saved = a3167_npr;
        np /= g; nq /= g;
        a3167_addprimes(x);
        a3167_rec(np, nq, x, k - 1);
        a3167_npr = saved;
    }
}

seq_t a(idx_t n) {
    a3167_cnt = 0;
    a3167_npr = 1;
    a3167_pr[0] = 2;
    a3167_rec(1, 2, 1, n);
    return (seq_t)a3167_cnt;
}
