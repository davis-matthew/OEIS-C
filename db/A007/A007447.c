/*
 * A007447  Logarithm of e.g.f. for primes.
 * https://oeis.org/A007447
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   E.g.f. A(x) = log(B(x)), B(x) = 1 + Sum_{k>=1} prime(k)*x^k/k!,
 *            so B' = A'*B gives a(n) = prime(n) - Sum_{k=1..n-1}
 *            binomial(n-1,k-1)*a(k)*prime(n-k), a(1) = 2
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 12, i64 n <= 20, i128 n <= 33,
 *            big any n
 * Helpers:   alloc_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"

static uint64_t *primes = NULL;
static idx_t nprimes = 0, capprimes = 0;

static uint64_t nth_prime(idx_t k) {
    while (nprimes < k) {
        uint64_t m = nprimes == 0 ? 2 : primes[nprimes - 1] + 1;
        for (;; m++) {
            bool ok = true;
            for (idx_t j = 0; j < nprimes && primes[j] * primes[j] <= m; j++) {
                if (m % primes[j] == 0) { ok = false; break; }
            }
            if (ok) break;
        }
        if (nprimes == capprimes) {
            capprimes = capprimes ? 2 * capprimes : 64;
            primes = (uint64_t *)realloc(primes, capprimes * sizeof(uint64_t));
        }
        primes[nprimes++] = m;
    }
    return primes[k - 1];
}

static seq_t *regrow(seq_t *old, idx_t oldlen, idx_t newlen) {
    seq_t *nv = alloc_zero(newlen);
    for (idx_t i = 0; i < oldlen; i++) nv[i] = old[i];
    if (old) release(old, oldlen);
    return nv;
}

seq_t a(idx_t n) {
    static seq_t *cache = NULL;   /* cache[k] = a(k) */
    static seq_t *p = NULL;       /* p[k] = prime(k) */
    static seq_t *row = NULL;     /* row[j] = binomial(maxn-1, j) */
    static idx_t maxn = 0;
    static idx_t cap = 0;         /* allocated length of each array */

    if (n <= maxn) return cache[n];

    if (n + 1 > cap) {
        idx_t newcap = cap ? cap : 16;
        while (newcap < n + 1) newcap *= 2;
        cache = regrow(cache, cap, newcap);
        p = regrow(p, cap, newcap);
        row = regrow(row, cap, newcap);
        cap = newcap;
    }

    for (idx_t i = maxn + 1; i <= n; i++) {
        p[i] = (seq_t)nth_prime(i);
        /* advance row to binomial(i-1, .) */
        if (i == 1) {
            row[0] = 1;
        } else {
            row[i - 1] = 1;
            for (idx_t j = i - 2; j >= 1; j--) row[j] += row[j - 1];
        }
        seq_t s = p[i];
        for (idx_t k = 1; k < i; k++) {
            s -= row[k - 1] * cache[k] * p[i - k];
        }
        cache[i] = s;
    }

    maxn = n;
    return cache[n];
}
