/*
 * A000446  Smallest number that is the sum of 2 squares (allowing zeros) in
 *          exactly n ways.
 * https://oeis.org/A000446
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 0; for n > 1, a(n) = min(A018782(2n-1), A018782(2n)),
 *            where A018782(k) is the smallest m = Product p_i^(d_i-1) with
 *            p_i the primes == 1 (mod 4) in increasing order and d_1 >= d_2
 *            >= ... a factorization of k (search over the multiplicative
 *            partitions of k)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 18, u64 n <= 30, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>

#define A446_MAXP 64

static unsigned a446_pr[A446_MAXP];
static double a446_lp[A446_MAXP];
static int a446_init = 0;
static int a446_ex[A446_MAXP];
static double a446_best;
static double a446_tol;
static seq_t a446_val;
static int a446_have;

static void a446_setup(void) {
    int c = 0;
    unsigned p;
    if (a446_init) return;
    for (p = 5; c < A446_MAXP; p += 4) {
        unsigned q;
        int ok = 1;
        for (q = 2; q * q <= p; q++)
            if (p % q == 0) { ok = 0; break; }
        if (ok) {
            a446_pr[c] = p;
            a446_lp[c] = log((double)p);
            c++;
        }
    }
    a446_init = 1;
}

static void a446_rec(uint64_t rem, uint64_t maxd, int idx, double lg, int mode) {
    uint64_t d, lim;
    if (rem == 1) {
        if (mode == 0) {
            if (lg < a446_best) a446_best = lg;
        } else if (lg <= a446_best + a446_tol) {
            seq_t v = 1;
            int i, e;
            for (i = 0; i < idx; i++)
                for (e = 0; e < a446_ex[i]; e++)
                    v *= (seq_t)a446_pr[i];
            if (!a446_have || v < a446_val) {
                a446_val = v;
                a446_have = 1;
            }
        }
        return;
    }
    if (idx >= A446_MAXP) return;
    if (mode == 0 && lg >= a446_best) return;
    if (mode == 1 && lg > a446_best + a446_tol) return;
    lim = maxd < rem ? maxd : rem;
    for (d = 2; d <= lim; d++) {
        if (rem % d == 0) {
            a446_ex[idx] = (int)(d - 1);
            a446_rec(rem / d, d, idx + 1, lg + (double)(d - 1) * a446_lp[idx], mode);
        }
    }
}

seq_t a(idx_t n) {
    uint64_t k1, k2;
    if (n <= 1) return 0;
    a446_setup();
    k1 = 2 * (uint64_t)n - 1;
    k2 = 2 * (uint64_t)n;
    a446_best = 1e300;
    a446_rec(k1, k1, 0, 0.0, 0);
    a446_rec(k2, k2, 0, 0.0, 0);
    a446_tol = 1e-9 * a446_best + 1e-9;
    a446_have = 0;
    a446_val = 0;
    a446_rec(k1, k1, 0, 0.0, 1);
    a446_rec(k2, k2, 0, 0.0, 1);
    return a446_val;
}
