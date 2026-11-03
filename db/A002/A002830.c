/*
 * A002830  Number of 3-edge-colored trivalent graphs with 2n nodes.
 * https://oeis.org/A002830
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   G.f. exp(Sum_{k>=1} F(x^k)/k), F = g.f. of A002831; equivalently
 *            (Burnside) a(n) = (1/(2n)!) * Sum_{s in S_{2n}} I(s)^3, where
 *            I(s) is the number of fixed-point-free involutions commuting
 *            with s. Grouping by cycle type gives Sum_n a(n) z^n = Prod_{k
 *            odd} Phi(k z^k) * Prod_{k=2l} G_k(z^l), where P...
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 17, u128 n <= 29,
 *            big any n
 * Validated: standard, pass, 37 terms through n = 170 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static uint64_t big_mod(const uint32_t *x, size_t len, uint64_t p) {
    uint64_t r = 0;
    size_t i = len;
    while (i > 0) {
        i--;
        r = ((r << 32) + x[i]) % p;
    }
    return r;
}

/* sum_{i=lo}^{hi} x[i]*y[m-i] mod p, values < 2^30 */
static uint64_t conv_at(const uint64_t *x, const uint64_t *y, idx_t m, idx_t lo, idx_t hi, uint64_t p) {
    uint64_t s = 0;
    int cnt = 0;
    idx_t i;
    for (i = lo; i <= hi; i++) {
        s += x[i] * y[m - i];
        if (++cnt == 15) { s %= p; cnt = 0; }
    }
    return s % p;
}

static void log_series(const uint64_t *F, uint64_t *g, uint64_t *ig, idx_t len,
                       const uint64_t *inv, uint64_t p) {
    idx_t m;
    g[0] = 0;
    ig[0] = 0;
    for (m = 1; m < len; m++) {
        uint64_t S = (m >= 2) ? conv_at(ig, F, m, 1, m - 1, p) : 0;
        uint64_t v = (m % p) * F[m] % p;
        ig[m] = (v + p - S) % p;
        g[m] = ig[m] * inv[m] % p;
    }
}

static int pr_isprime(uint64_t x) {
    uint64_t d;
    if (x % 2 == 0) return 0;
    for (d = 3; d * d <= x; d += 2)
        if (x % d == 0) return 0;
    return 1;
}

static uint64_t pw_mod(uint64_t b, uint64_t e, uint64_t p) {
    uint64_t r = 1;
    b %= p;
    while (e) {
        if (e & 1) r = r * b % p;
        b = b * b % p;
        e >>= 1;
    }
    return r;
}

static uint64_t solve_mod(idx_t n, uint64_t p) {
    idx_t i, j, k, l, c, m;
    uint64_t *inv = (uint64_t *)malloc((2 * n + 2) * sizeof(uint64_t));
    uint64_t *F = (uint64_t *)malloc((n + 1) * sizeof(uint64_t));
    uint64_t *g = (uint64_t *)malloc((n + 1) * sizeof(uint64_t));
    uint64_t *ig = (uint64_t *)malloc((n + 1) * sizeof(uint64_t));
    uint64_t *s = (uint64_t *)malloc((n + 1) * sizeof(uint64_t));
    uint64_t *L = (uint64_t *)calloc(n + 1, sizeof(uint64_t));
    uint64_t res;

    inv[1] = 1;
    for (i = 2; i <= 2 * n; i++)
        inv[i] = (p - (p / i) * inv[p % i] % p) % p;

    /* Phi coefficients and their log */
    F[0] = 1;
    for (j = 1; j <= n; j++) {
        uint64_t o = (2 * j - 1) % p;
        F[j] = F[j - 1] * o % p * o % p * inv[2 * j] % p;
    }
    log_series(F, s, ig, n + 1, inv, p);
    for (k = 1; k <= n; k += 2) {
        uint64_t kp = k % p, pw = 1;
        for (j = 1; j * k <= n; j++) {
            pw = pw * kp % p;
            L[j * k] = (L[j * k] + s[j] * pw) % p;
        }
    }

    /* even cycle lengths k = 2l */
    for (l = 1; l <= n; l++) {
        idx_t len = n / l + 1;
        uint64_t kk = (2 * l) % p, invk = inv[2 * l];
        uint64_t t2 = 1, t1 = 1, r = 1;
        F[0] = 1;
        for (c = 1; c < len; c++) {
            uint64_t t;
            if (c == 1) t = 1;
            else {
                t = (t1 + ((c - 1) % p) * kk % p * t2) % p;
                t2 = t1;
                t1 = t;
            }
            r = r * invk % p * inv[c] % p;
            F[c] = t * t % p * t % p * r % p;
        }
        log_series(F, g, ig, len, inv, p);
        for (c = 1; c < len; c++)
            L[l * c] = (L[l * c] + g[c]) % p;
    }

    /* exp */
    for (i = 1; i <= n; i++) ig[i] = (i % p) * L[i] % p;
    ig[0] = 0;
    F[0] = 1;
    for (m = 1; m <= n; m++)
        F[m] = conv_at(ig, F, m, 1, m, p) * inv[m] % p;
    res = F[n];

    free(inv); free(F); free(g); free(ig); free(s); free(L);
    return res;
}

static uint32_t *pr_list = NULL;
static size_t pr_cnt = 0, pr_cap = 0;

static uint64_t pr_get(size_t i) {
    while (pr_cnt <= i) {
        uint64_t c = pr_cnt ? (uint64_t)pr_list[pr_cnt - 1] - 2 : ((uint64_t)1 << 30) - 1;
        while (!pr_isprime(c)) c -= 2;
        if (pr_cnt == pr_cap) {
            size_t nc = pr_cap ? pr_cap * 2 : 64;
            uint32_t *np = (uint32_t *)malloc(nc * sizeof(uint32_t));
            if (pr_cnt) memcpy(np, pr_list, pr_cnt * sizeof(uint32_t));
            free(pr_list);
            pr_list = np;
            pr_cap = nc;
        }
        pr_list[pr_cnt++] = (uint32_t)c;
    }
    return pr_list[i];
}

seq_t a(idx_t n) {
    double mm = 1.0;
    long e = 0;
    idx_t i;
    long bits;
    size_t np, q, LEN, t;
    uint32_t *R, *Q;
    seq_t res = 0;

    if (n == 0) return 1;

    /* bit-size estimate of ((2n-1)!!)^3 / (2n)! */
    for (i = 1; i <= 2 * n; i++) {
        int rep;
        if (i & 1) {
            for (rep = 0; rep < 3; rep++) {
                mm *= (double)i;
                while (mm >= 2.0) { mm /= 2.0; e++; }
            }
        }
        mm /= (double)i;
        while (mm < 1.0) { mm *= 2.0; e--; }
        while (mm >= 2.0) { mm /= 2.0; e++; }
    }
    bits = e + 1 + 24;
    if (bits < 1) bits = 1;
    np = (size_t)(bits / 29) + 2;

    LEN = np + 2;
    R = (uint32_t *)calloc(LEN, sizeof(uint32_t));
    Q = (uint32_t *)calloc(LEN, sizeof(uint32_t));
    Q[0] = 1;

    for (q = 0; q < np; q++) {
        uint64_t p = pr_get(q);
        uint64_t r = solve_mod(n, p);
        uint64_t rm = big_mod(R, LEN, p);
        uint64_t qm = big_mod(Q, LEN, p);
        uint64_t tt = (r + p - rm) % p * pw_mod(qm, p - 2, p) % p;
        uint64_t carry = 0;
        for (t = 0; t < LEN; t++) {
            uint64_t cur = (uint64_t)R[t] + (uint64_t)Q[t] * tt + carry;
            R[t] = (uint32_t)cur;
            carry = cur >> 32;
        }
        carry = 0;
        for (t = 0; t < LEN; t++) {
            uint64_t cur = (uint64_t)Q[t] * p + carry;
            Q[t] = (uint32_t)cur;
            carry = cur >> 32;
        }
    }

    for (t = LEN; t > 0; t--) {
        res = res * 65536 + (seq_t)(int)(R[t - 1] >> 16);
        res = res * 65536 + (seq_t)(int)(R[t - 1] & 0xFFFFu);
    }
    free(R);
    free(Q);
    return res;
}
