/*
 * A003227  Endpoints (leaves) in rooted trees with n nodes.
 * https://oeis.org/A003227
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=1..n-1} A000081(i) * S(n-i) for n >= 2, a(1) = 1,
 *            where S(m) = Sum_{d|m} a(d); this comes from the g.f. L(x) = x +
 *            R(x)*Sum_{k>=1} L(x^k), R(x) = g.f. of A000081. A000081 itself:
 *            r(1) = 1, r(N+1) = (1/N) * Sum_{j=1..N} (Sum_{d|j} d*r(d)) *
 *            r(N-j+1)
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 23, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

#define A3227_CAP 10050

static seq_t A3227_R[A3227_CAP + 1];   /* A000081 */
static seq_t A3227_Sr[A3227_CAP + 1];  /* Sum_{d|k} d*r(d) */
static seq_t A3227_A[A3227_CAP + 1];   /* a(k) */
static seq_t A3227_SA[A3227_CAP + 1];  /* Sum_{d|k} a(d) */
static idx_t A3227_nr = 0, A3227_na = 0;
static int A3227_fixed = -1;

static int A3227_is_fixed(void) {
    if (A3227_fixed < 0) {
        seq_t x = 1;
        int i;
        for (i = 0; i < 140; i++) x *= 2;
        A3227_fixed = (x == 0) ? 1 : 0;
    }
    return A3227_fixed;
}

static void A3227_ensure_r(idx_t m) {
    while (A3227_nr < m) {
        idx_t k = A3227_nr + 1, j, mm;
        seq_t r;
        if (k == 1) {
            r = 1;
        } else {
            idx_t N = k - 1;
            if (A3227_is_fixed()) {
                /* exact division by N done via quotient/remainder parts */
                seq_t Q = 0;
                uint64_t Rm = 0;
                for (j = 1; j <= N; j++) {
                    seq_t s = A3227_Sr[j], t = A3227_R[N - j + 1];
                    seq_t qs = s / N, qt = t / N;
                    uint64_t rs = (uint64_t)(s % N), rt = (uint64_t)(t % N);
                    Q += qs * qt * N + qs * rt + qt * rs;
                    Rm += rs * rt;
                }
                r = Q + (seq_t)(Rm / N);
            } else {
                seq_t tot = 0;
                for (j = 1; j <= N; j++) tot += A3227_Sr[j] * A3227_R[N - j + 1];
                r = tot / N;
            }
        }
        A3227_R[k] = r;
        A3227_nr = k;
        {
            seq_t kr = r * k;
            for (mm = k; mm <= A3227_CAP; mm += k) A3227_Sr[mm] += kr;
        }
    }
}

static void A3227_ensure_a(idx_t n) {
    while (A3227_na < n) {
        idx_t k = A3227_na + 1, i, mm;
        seq_t v;
        if (k == 1) {
            v = 1;
        } else {
            A3227_ensure_r(k - 1);
            v = 0;
            for (i = 1; i < k; i++) v += A3227_R[i] * A3227_SA[k - i];
        }
        A3227_A[k] = v;
        A3227_na = k;
        for (mm = k; mm <= A3227_CAP; mm += k) A3227_SA[mm] += v;
    }
}

seq_t a(idx_t n) {
    if (n < 1 || n > A3227_CAP) return 0;
    A3227_ensure_a(n);
    return A3227_A[n];
}
