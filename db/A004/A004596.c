/*
 * A004596  Expansion of e in base 5.
 * https://oeis.org/A004596
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, cons
 * Formula:   base-5 digits of e = 2 + sum_{k>=2} 1/k!, extracted by the
 *            mixed-radix spigot algorithm (multiply the digits of the
 *            factorial-base fraction by 5 and propagate carries); a(1) = 2
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>

static unsigned char *dig = NULL;
static idx_t ndig = 0;

static void compute_digits(idx_t N) {
    /* need m with log5(m!) > N + guard */
    double target = (double)N + 30.0, s = 0.0, l5 = log(5.0);
    idx_t m = 2;
    while (s < target) { m++; s += log((double)m) / l5; }
    m += 10;
    unsigned int *r = (unsigned int *)malloc((size_t)(m + 1) * sizeof(unsigned int));
    for (idx_t i = 0; i <= m; i++) r[i] = 1;
    unsigned char *d = (unsigned char *)malloc((size_t)(N + 1) * sizeof(unsigned char));
    d[0] = 0;
    d[1] = 2;
    for (idx_t j = 2; j <= N; j++) {
        unsigned long carry = 0;
        for (idx_t i = m; i >= 2; i--) {
            unsigned long x = 5UL * r[i] + carry;
            r[i] = (unsigned int)(x % i);
            carry = x / i;
        }
        d[j] = (unsigned char)carry;
    }
    free(r);
    free(dig);
    dig = d;
    ndig = N;
}

seq_t a(idx_t n) {
    if (n > ndig) {
        idx_t N = n < 64 ? 64 : n;
        if (N < 2 * ndig) N = 2 * ndig;
        compute_digits(N);
    }
    return (seq_t)dig[n];
}
