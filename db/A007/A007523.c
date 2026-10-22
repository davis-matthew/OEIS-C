/*
 * A007523  Primes in A092845 (decimal expansion of Pi written backwards).
 * https://oeis.org/A007523
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = n-th prime in A092845 (reversals of the first k digits of
 *            Pi): search over k, with Pi's digits made by a spigot
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   seq_is_fixed_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seq_is_fixed_width.h"

#define PI_N 820
#define PI_USE 780

static char pi_buf[PI_N + 4];
static int pi_cnt = 0;
static bool pi_ready = false;

static void pi_emit(int d) {
    if (pi_cnt < PI_N + 3) pi_buf[pi_cnt++] = (char)d;
}

static void gen_pi(void) {
    int len = PI_N * 10 / 3 + 1;
    int *A = (int *)malloc((size_t)len * sizeof(int));
    int i, j, k, nines = 0, predigit = 0;
    for (i = 0; i < len; i++) A[i] = 2;
    for (j = 1; j <= PI_N; j++) {
        long long q = 0;
        for (i = len; i > 0; i--) {
            long long x = 10LL * A[i - 1] + q * i;
            A[i - 1] = (int)(x % (2 * i - 1));
            q = x / (2 * i - 1);
        }
        A[0] = (int)(q % 10);
        q /= 10;
        if (q == 9) {
            nines++;
        } else if (q == 10) {
            pi_emit(predigit + 1);
            for (k = 0; k < nines; k++) pi_emit(0);
            predigit = 0;
            nines = 0;
        } else {
            pi_emit(predigit);
            predigit = (int)q;
            for (k = 0; k < nines; k++) pi_emit(9);
            nines = 0;
        }
    }
    pi_emit(predigit);
    free(A);
    pi_ready = true;
}

/* digit k (1-based) of Pi: 3,1,4,1,5,... ; pi_buf[0] is a leading 0 */
static int pi_digit(int k) { return pi_buf[k]; }

static seq_t addmod(seq_t a, seq_t b, seq_t m) {
    return (a >= m - b) ? a - (m - b) : a + b;
}

static seq_t mulmod(seq_t a, seq_t b, seq_t m) {
    seq_t r = 0;
    if (!seq_is_fixed_width()) return a * b % m;
    a %= m;
    b %= m;
    while (b > 0) {
        if ((b & 1) != 0) r = addmod(r, a, m);
        a = addmod(a, a, m);
        b >>= 1;
    }
    return r;
}

static seq_t powmod(seq_t b, seq_t e, seq_t m) {
    seq_t r = 1;
    b %= m;
    while (e > 0) {
        if ((e & 1) != 0) r = mulmod(r, b, m);
        b = mulmod(b, b, m);
        e >>= 1;
    }
    return r % m;
}

static int small_primes[200];
static int n_small = 0;

static void init_small(void) {
    int p, q;
    for (p = 2; n_small < 200; p++) {
        bool ok = true;
        for (q = 2; q * q <= p; q++)
            if (p % q == 0) { ok = false; break; }
        if (ok) small_primes[n_small++] = p;
    }
}

static bool is_prime_seq(seq_t x) {
    static const int wit[12] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
    seq_t d, xm1;
    int r = 0, i, w;
    if (x < 2) return false;
    for (i = 0; i < n_small; i++) {
        int p = small_primes[i];
        if (x == p) return true;
        if (x % p == 0) return false;
    }
    xm1 = x - 1;
    d = xm1;
    while ((d & 1) == 0) { d >>= 1; r++; }
    for (w = 0; w < 12; w++) {
        seq_t y = powmod((seq_t)wit[w], d, x);
        bool comp;
        if (y == 1 || y == xm1) continue;
        comp = true;
        for (i = 1; i < r; i++) {
            y = mulmod(y, y, x);
            if (y == xm1) { comp = false; break; }
        }
        if (comp) return false;
    }
    return true;
}

static seq_t terms[64];
static int nterms = 0;
static int scan_k = 0;      /* digits of Pi consumed so far */
static bool scan_done = false;
static bool st_init = false;
static seq_t cur_rev;       /* reversal of first scan_k digits */
static seq_t cur_pow;       /* 10^scan_k */

seq_t a(idx_t n) {
    if (!st_init) {
        gen_pi();
        init_small();
        cur_rev = 0;
        cur_pow = 1;
        st_init = true;
    }
    while ((idx_t)nterms < n && !scan_done) {
        int k = scan_k + 1;
        int d;
        seq_t t, s;
        if (k > PI_USE || nterms >= 64) { scan_done = true; break; }
        d = pi_digit(k);
        t = cur_pow * d;
        if (d != 0 && t / d != cur_pow) { scan_done = true; break; }
        s = t + cur_rev;
        if (s < t) { scan_done = true; break; }
        cur_rev = s;
        scan_k = k;
        if (is_prime_seq(cur_rev)) terms[nterms++] = cur_rev;
        {
            seq_t np = cur_pow * 10;
            if (np / 10 != cur_pow) {
                scan_done = true; /* next candidate cannot fit */
            } else {
                cur_pow = np;
            }
        }
    }
    if (n >= 1 && (idx_t)nterms >= n) return terms[n - 1];
    return 0;
}
