/*
 * A000751  Boustrophedon transform of partition numbers.
 * https://oeis.org/A000751
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Boustrophedon transform of b(k) = A000041(k), the partition
 *            numbers: T(n,0) = b(n), T(n,k) = T(n,k-1) + T(n-1,n-k) for 1 <=
 *            k <= n, a(n) = T(n,n). Partition numbers come from Euler's
 *            pentagonal recurrence, p(0) = 1
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 14, u64 n <= 23, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

#define A751_MAX 10050

static seq_t a751_P[A751_MAX];
static seq_t a751_A[A751_MAX];
static seq_t a751_R0[A751_MAX];
static seq_t a751_R1[A751_MAX];
static idx_t a751_pcnt = 0;
static idx_t a751_rcnt = 0;

static void a751_parts(idx_t m) {
    while (a751_pcnt <= m) {
        idx_t i = a751_pcnt;
        if (i == 0) {
            a751_P[0] = 1;
        } else {
            seq_t pos = 0, neg = 0;
            idx_t k;
            for (k = 1;; k++) {
                idx_t g1 = k * (3 * k - 1) / 2;
                idx_t g2 = k * (3 * k + 1) / 2;
                if (g1 > i) break;
                if (k & 1) {
                    pos += a751_P[i - g1];
                    if (g2 <= i) pos += a751_P[i - g2];
                } else {
                    neg += a751_P[i - g1];
                    if (g2 <= i) neg += a751_P[i - g2];
                }
            }
            a751_P[i] = pos - neg;
        }
        a751_pcnt++;
    }
}

seq_t a(idx_t n) {
    static seq_t *prev = a751_R0;
    static seq_t *cur = a751_R1;
    if (n >= A751_MAX) return 0;
    a751_parts(n);
    while (a751_rcnt <= n) {
        idx_t i = a751_rcnt;
        if (i == 0) {
            prev[0] = a751_P[0];
            a751_A[0] = prev[0];
        } else {
            seq_t *t;
            idx_t j;
            cur[0] = a751_P[i];
            for (j = 1; j <= i; j++) {
                cur[j] = cur[j - 1] + prev[i - j];
            }
            a751_A[i] = cur[i];
            t = prev;
            prev = cur;
            cur = t;
        }
        a751_rcnt++;
    }
    return a751_A[n];
}
