/*
 * A006632  a(n) = 3*binomial(4*n-1, n-1)/(4*n-1).
 * https://oeis.org/A006632
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*binomial(4*n-1, n-1)/(4*n-1) =
 *            3*(4n-2)!/((n-1)!*(3n)!), evaluated by prime factorization
 *            (Legendre's formula)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 22, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static unsigned char *sieve_comp = NULL;
static uint64_t sieve_lim = 0;

static void ensure_sieve(uint64_t lim) {
    uint64_t i, j;
    if (lim <= sieve_lim && sieve_comp) return;
    free(sieve_comp);
    sieve_comp = (unsigned char *)malloc((size_t)(lim + 1));
    memset(sieve_comp, 0, (size_t)(lim + 1));
    sieve_comp[0] = 1;
    if (lim >= 1) sieve_comp[1] = 1;
    for (i = 2; i * i <= lim; i++)
        if (!sieve_comp[i])
            for (j = i * i; j <= lim; j += i) sieve_comp[j] = 1;
    sieve_lim = lim;
}

static uint64_t vfact(uint64_t m, uint64_t p) {
    uint64_t e = 0;
    while (m) {
        m /= p;
        e += m;
    }
    return e;
}

seq_t a(idx_t n) {
    uint64_t lim = 4 * (uint64_t)n - 2;
    uint64_t p, k;
    seq_t r = 1;
    if (lim < 3 * (uint64_t)n) lim = 3 * (uint64_t)n;
    ensure_sieve(lim);
    for (p = 2; p <= lim; p++) {
        long long e;
        if (sieve_comp[p]) continue;
        e = (long long)vfact(4 * (uint64_t)n - 2, p)
          - (long long)vfact((uint64_t)n - 1, p)
          - (long long)vfact(3 * (uint64_t)n, p);
        if (p == 3) e += 1;
        for (k = 0; (long long)k < e; k++) r *= (seq_t)p;
    }
    return r;
}
