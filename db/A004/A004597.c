/*
 * A004597  Expansion of e in base 6.
 * https://oeis.org/A004597
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, cons
 * Formula:   a(1) = 2; a(n) for n >= 2 is the (n-1)-th base-6 digit of
 *            frac(e), computed exactly with the spigot algorithm: frac(e) =
 *            Sum_{k>=2} 1/k! is held in mixed radix (all digits 1),
 *            repeatedly multiplied by 6, and the carry out is the next digit
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static unsigned char *a004597_dig = NULL;  /* a004597_dig[i] = i-th fractional digit, i>=1 */
static idx_t a004597_cnt = 0;

static void a004597_compute(idx_t N) {
    /* choose m with m! > 6^(N+12); floor(log2 k) sums underestimate log2(m!) */
    unsigned long long need = (unsigned long long)(N + 12) * 3ULL, acc = 0;
    long m = 2;
    while (acc < need) {
        m++;
        long k = m, b = 0;
        while (k > 1) { k >>= 1; b++; }
        acc += (unsigned long long)b;
    }
    m += 4;
    int *d = (int *)malloc(sizeof(int) * (size_t)(m + 1));
    long k;
    idx_t i;
    for (k = 0; k <= m; k++) d[k] = 1;
    unsigned char *res = (unsigned char *)malloc((size_t)N + 1);
    res[0] = 2;
    for (i = 1; i <= N; i++) {
        int carry = 0;
        for (k = m; k >= 2; k--) {
            int t = d[k] * 6 + carry;
            d[k] = t % (int)k;
            carry = t / (int)k;
        }
        res[i] = (unsigned char)carry;
    }
    free(d);
    free(a004597_dig);
    a004597_dig = res;
    a004597_cnt = N;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    if (n == 1) return (seq_t)2;
    idx_t f = n - 1;
    if (f > a004597_cnt) {
        idx_t N = a004597_cnt * 2;
        if (N < f) N = f;
        if (N < 64) N = 64;
        a004597_compute(N);
    }
    return (seq_t)a004597_dig[f];
}
