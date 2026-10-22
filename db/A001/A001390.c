/*
 * A001390  Smallest multiplicative generator for quadratic residues mod
 *          prime(n).
 * https://oeis.org/A001390
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search: a(n) = least a with multiplicative order (p-1)/2 mod p =
 *            prime(n) (a(1) = 1)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a1390_powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/a1390_powmod.h"

static uint64_t *a1390_primes = NULL;
static uint64_t a1390_cnt = 0;
static uint64_t a1390_lim = 0;

static void a1390_sieve(uint64_t lim) {
    char *c = (char *)calloc(lim + 1, 1);
    uint64_t i, j, k = 0;
    for (i = 2; i * i <= lim; i++)
        if (!c[i])
            for (j = i * i; j <= lim; j += i) c[j] = 1;
    for (i = 2; i <= lim; i++) if (!c[i]) k++;
    free(a1390_primes);
    a1390_primes = (uint64_t *)malloc((k + 1) * sizeof(uint64_t));
    k = 0;
    for (i = 2; i <= lim; i++) if (!c[i]) a1390_primes[k++] = i;
    a1390_cnt = k;
    a1390_lim = lim;
    free(c);
}

static uint64_t a1390_nth_prime(uint64_t n) {
    uint64_t lim = a1390_lim ? a1390_lim : 1024;
    while (a1390_cnt < n) {
        if (a1390_lim) lim *= 2;
        a1390_sieve(lim);
    }
    return a1390_primes[n - 1];
}

seq_t a(idx_t n) {
    uint64_t p = a1390_nth_prime((uint64_t)n);
    uint64_t q, t, f[64], x;
    int nf = 0, i;
    bool ok;
    if (p <= 3) return (seq_t)1;
    q = (p - 1) / 2;
    t = q;
    for (x = 2; x * x <= t; x++) {
        if (t % x == 0) {
            f[nf++] = x;
            while (t % x == 0) t /= x;
        }
    }
    if (t > 1) f[nf++] = t;
    for (x = 2; x < p; x++) {
        if (a1390_powmod(x, q, p) != 1) continue;
        ok = true;
        for (i = 0; i < nf; i++) {
            if (a1390_powmod(x, q / f[i], p) == 1) { ok = false; break; }
        }
        if (ok) return (seq_t)x;
    }
    return (seq_t)1;
}
