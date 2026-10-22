/*
 * A002133  Number of partitions of n with exactly two part sizes.
 * https://oeis.org/A002133
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look, easy
 * Formula:   a(n) = (Sum_{m=1..n-1} d(m)*d(n-m) - sigma(n) + d(n))/2, from
 *            G.f. (G(x)^2-H(x))/2 with G = Sum x^k/(1-x^k), H = Sum
 *            x^(2k)/(1-x^k)^2
 * Range:     u8 n <= 39, u16 n <= 1679, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static uint32_t *a002133_d = NULL;
static idx_t a002133_len = 0;

static void a002133_ensure(idx_t n) {
    idx_t i, j, len;
    if (n < a002133_len) return;
    len = n + 1;
    if (len < 2 * a002133_len) len = 2 * a002133_len;
    free(a002133_d);
    a002133_d = (uint32_t *)calloc((size_t)len, sizeof(uint32_t));
    for (i = 1; i < len; i++)
        for (j = i; j < len; j += i)
            a002133_d[j]++;
    a002133_len = len;
}

seq_t a(idx_t n) {
    uint64_t conv = 0, sig = 0, dn = 0;
    idx_t m, k;
    if (n < 3) return (seq_t)0;
    a002133_ensure(n);
    for (m = 1; m < n; m++)
        conv += (uint64_t)a002133_d[m] * (uint64_t)a002133_d[n - m];
    for (k = 1; k * k <= n; k++) {
        if (n % k == 0) {
            idx_t q = n / k;
            sig += k;
            dn++;
            if (q != k) {
                sig += q;
                dn++;
            }
        }
    }
    return (seq_t)((conv + dn - sig) / 2);
}
