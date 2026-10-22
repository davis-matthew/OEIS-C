/*
 * A004598  Expansion of e in base 7.
 * https://oeis.org/A004598
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, cons
 * Formula:   a(1) = 2; a(n) = n-th digit of e in base 7, from a mixed-radix
 *            spigot: e - 2 = sum_{k>=2} 1/k!; multiply by 7 repeatedly and
 *            propagate carries from radix N down to radix 2
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static unsigned char *e7_digits = NULL; /* e7_digits[k-1] = k-th fractional digit */
static size_t e7_have = 0;

static void e7_compute(size_t m) {
    /* choose N so that N! > 7^(m+20) */
    double x = 1.0;
    size_t cnt = 0, N = 1;
    while (cnt < m + 20) {
        N++;
        x *= (double)N;
        while (x >= 7.0) { x /= 7.0; cnt++; }
    }
    unsigned int *d = (unsigned int *)malloc((N + 1) * sizeof(unsigned int));
    size_t i, k;
    for (i = 0; i <= N; i++) d[i] = 1;
    unsigned char *out = (unsigned char *)malloc(m);
    for (k = 0; k < m; k++) {
        unsigned long carry = 0;
        for (i = N; i >= 2; i--) {
            unsigned long t = (unsigned long)d[i] * 7UL + carry;
            d[i] = (unsigned int)(t % i);
            carry = t / i;
        }
        out[k] = (unsigned char)carry;
    }
    free(d);
    free(e7_digits);
    e7_digits = out;
    e7_have = m;
}

seq_t a(idx_t n) {
    if (n <= 1) return (seq_t)2;
    size_t need = (size_t)(n - 1);
    if (need > e7_have) {
        size_t m = 2 * e7_have;
        if (m < need) m = need;
        if (m < 64) m = 64;
        e7_compute(m);
    }
    return (seq_t)e7_digits[need - 1];
}
