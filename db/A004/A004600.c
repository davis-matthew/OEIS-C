/*
 * A004600  Expansion of e in base 9.
 * https://oeis.org/A004600
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, cons
 * Formula:   a(1) = 2; for n >= 2, a(n) = floor(9^(n-1) * e) mod 9. Digits
 *            come from the mixed-radix spigot e = 2 + Sum_{k>=2} 1/k!:
 *            multiply the fraction by 9 and carry from k = m down to 2
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>

static unsigned char *a004600_dig = NULL;
static idx_t a004600_len = 0; /* digits valid for indices 1..a004600_len */

static void a004600_compute(idx_t N) {
    /* choose m with log(m!) > (N + 30) * log(9) so truncation cannot affect digits */
    double target = ((double)N + 30.0) * log(9.0);
    double lf = 0.0;
    unsigned long m = 1;
    while (lf < target) { m++; lf += log((double)m); }
    m += 10;

    unsigned long *r = (unsigned long *)malloc((m + 1) * sizeof(unsigned long));
    unsigned char *d = (unsigned char *)malloc((size_t)(N + 1));
    for (unsigned long k = 0; k <= m; k++) r[k] = 1;
    d[0] = 0;
    d[1] = 2;
    for (idx_t i = 2; i <= N; i++) {
        unsigned long carry = 0;
        for (unsigned long k = m; k >= 2; k--) {
            unsigned long x = r[k] * 9UL + carry;
            r[k] = x % k;
            carry = x / k;
        }
        d[i] = (unsigned char)carry;
    }
    free(r);
    free(a004600_dig);
    a004600_dig = d;
    a004600_len = N;
}

seq_t a(idx_t n) {
    if (n > a004600_len) {
        idx_t N = a004600_len * 2;
        if (N < n + 16) N = n + 16;
        if (N < 256) N = 256;
        a004600_compute(N);
    }
    return (seq_t)a004600_dig[n];
}
