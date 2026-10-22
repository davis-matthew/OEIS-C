/*
 * A000448  Smallest number that is the sum of 2 squares in at least n ways.
 * https://oeis.org/A000448
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 0; for n >= 2, a(n) is the least m whose number of
 *            representations m = x^2 + y^2 with 0 <= x <= y, ceil(B(m)/2), is
 *            at least n. Here B(m) = Product (e_i + 1) over the primes p_i ==
 *            1 (mod 4) dividing m. So m = Product p_i^e_i with nonincreasing
 *            exponents on 5, 13, 17, 29, ..., and B >= 2n-...
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 72, u64 n <= 2560,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>

#define A448_NP 64

static int a448_np = 0;
static unsigned long long a448_pr[A448_NP];
static long double a448_lg[A448_NP];
static int a448_cur[A448_NP], a448_best[A448_NP];
static int a448_bestlen;
static long double a448_bestlog;
static unsigned long long a448_target;

static void a448_init(void) {
    unsigned long long c;
    if (a448_np) return;
    for (c = 5; a448_np < A448_NP; c += 4) {
        unsigned long long d;
        int prime = 1;
        for (d = 2; d * d <= c; d++)
            if (c % d == 0) { prime = 0; break; }
        if (prime) {
            a448_pr[a448_np] = c;
            a448_lg[a448_np] = logl((long double)c);
            a448_np++;
        }
    }
}

static seq_t a448_val(const int *ex, int len) {
    seq_t v = 1;
    int i, j;
    for (i = 0; i < len; i++)
        for (j = 0; j < ex[i]; j++)
            v *= (seq_t)a448_pr[i];
    return v;
}

static void a448_set_best(int len) {
    int i;
    for (i = 0; i < len; i++) a448_best[i] = a448_cur[i];
    a448_bestlen = len;
}

static void a448_dfs(int i, unsigned long long maxe, long double lv,
                     unsigned long long B) {
    unsigned long long e;
    if (B >= a448_target) {
        if (lv < a448_bestlog - 1e-12L) {
            a448_bestlog = lv;
            a448_set_best(i);
        } else if (lv < a448_bestlog + 1e-12L) {
            if (a448_val(a448_cur, i) < a448_val(a448_best, a448_bestlen)) {
                a448_bestlog = lv;
                a448_set_best(i);
            }
        }
        return;
    }
    if (i >= a448_np) return;
    for (e = 1; e <= maxe; e++) {
        long double nl = lv + (long double)e * a448_lg[i];
        if (nl > a448_bestlog + 1e-12L) break;
        a448_cur[i] = (int)e;
        a448_dfs(i + 1, e, nl, B * (e + 1));
    }
}

seq_t a(idx_t n) {
    if (n <= 1) return 0;
    a448_init();
    a448_target = 2 * (unsigned long long)n - 1;
    a448_bestlog = 1e300L;
    a448_bestlen = 0;
    a448_dfs(0, a448_target - 1, 0.0L, 1);
    return a448_val(a448_best, a448_bestlen);
}
