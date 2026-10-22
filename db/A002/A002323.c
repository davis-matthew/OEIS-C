/*
 * A002323  ((2^m - 1) / p) mod p, where p = prime(n) and m = ord(2,p).
 * https://oeis.org/A002323
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((2^m - 1)/p) mod p, where p = prime(n) and m = ord(2,p);
 *            computed as (2^m mod p^2 - 1)/p
 * Range:     u8 n <= 56, u16 n <= 6594, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a2323_mulmod, a2323_powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/a2323_mulmod.h"
#include "../utils/a2323_powmod.h"

static uint32_t *a2323_primes = NULL;
static size_t a2323_np = 0;

static void a2323_ensure(idx_t n) {
    size_t lim = 1024;
    while (a2323_np < n) {
        size_t i, j, cnt = 0;
        unsigned char *comp;
        lim *= 2;
        comp = (unsigned char *)calloc(lim + 1, 1);
        for (i = 2; i * i <= lim; i++)
            if (!comp[i])
                for (j = i * i; j <= lim; j += i) comp[j] = 1;
        for (i = 2; i <= lim; i++) if (!comp[i]) cnt++;
        free(a2323_primes);
        a2323_primes = (uint32_t *)malloc(cnt * sizeof(uint32_t));
        cnt = 0;
        for (i = 2; i <= lim; i++) if (!comp[i]) a2323_primes[cnt++] = (uint32_t)i;
        a2323_np = cnt;
        free(comp);
    }
}

seq_t a(idx_t n) {
    uint64_t p, m, t, q, r, p2;
    a2323_ensure(n);
    p = a2323_primes[n - 1];
    m = p - 1;
    t = m;
    for (q = 2; q * q <= t; q++) {
        if (t % q == 0) {
            while (t % q == 0) t /= q;
            while (m % q == 0 && a2323_powmod(2, m / q, p) == 1) m /= q;
        }
    }
    if (t > 1) {
        q = t;
        while (m % q == 0 && a2323_powmod(2, m / q, p) == 1) m /= q;
    }
    p2 = p * p;
    r = a2323_powmod(2, m, p2);
    return (seq_t)((r + p2 - 1) % p2 / p);
}
