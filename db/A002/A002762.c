/*
 * A002762  Number of bipartite partitions.
 * https://oeis.org/A002762
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{m=0..n} p(n-m)*r(m), where p = A000041 (partition
 *            numbers) and r(m) = round((m+3)^2/12) + floor(m/2) + 1 + (m+1) +
 *            1, i.e. G.f.: P(x) * ( 1/((1-x)(1-x^2)(1-x^3)) +
 *            1/((1-x)(1-x^2)) + 1/(1-x)^2 + 1/(1-x) ), with P(x) =
 *            Prod_{k>=1} 1/(1-x^k). p(n) is computed by Euler's pentagonal
 *            recur...
 * Range:     u8 n <= 6, u16 n <= 22, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   a001028_grow, a001028_z
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/a001028_z.h"
#include "../utils/a001028_grow.h"

static seq_t *a002762_p = NULL;
static idx_t a002762_len = 0;

static void a002762_ensure(idx_t n) {
    idx_t need = n + 1;
    idx_t i;
    if (need <= a002762_len) return;
    if (a002762_p == NULL) {
        a002762_p = a001028_z(need);
    } else {
        a002762_p = a001028_grow(a002762_p, a002762_len, need);
    }
    for (i = a002762_len; i < need; i++) {
        if (i == 0) {
            a002762_p[0] = 1;
            continue;
        }
        seq_t pos = 0, neg = 0;
        idx_t k;
        for (k = 1;; k++) {
            idx_t g1 = k * (3 * k - 1) / 2;
            idx_t g2 = k * (3 * k + 1) / 2;
            if (g1 > i) break;
            if (k & 1) {
                pos += a002762_p[i - g1];
                if (g2 <= i) pos += a002762_p[i - g2];
            } else {
                neg += a002762_p[i - g1];
                if (g2 <= i) neg += a002762_p[i - g2];
            }
        }
        a002762_p[i] = pos - neg;
    }
    a002762_len = need;
}

seq_t a(idx_t n) {
    idx_t m;
    seq_t sum = 0;
    a002762_ensure(n);
    for (m = 0; m <= n; m++) {
        uint64_t mm = (uint64_t)m;
        uint64_t q3 = ((mm + 3) * (mm + 3) + 6) / 12;
        uint64_t r = q3 + mm / 2 + 1 + (mm + 1) + 1;
        sum += a002762_p[n - m] * (seq_t)r;
    }
    return sum;
}
