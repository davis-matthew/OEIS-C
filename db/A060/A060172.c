/*
 * A060172  Number of orbits of length n under a map whose periodic points are
 *          counted by A027306.
 * https://oeis.org/A060172
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (1/n) * Σ_{d|n} μ(d) * A027306(n/d) (OEIS A060172)
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 31, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   binom, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/pow2.h"   /* seq_t pow2(idx_t exp);  */
#include "../utils/binom.h" /* seq_t binom(idx_t a, idx_t b); */

#define PRIMES_LIMIT 10000   /* sufficient for n ≤ 10 000 */

static int primes[PRIMES_LIMIT + 1];
static size_t primes_cnt = 0;
static int primes_built = 0;

/* Simple prime sieve up to PRIMES_LIMIT (called lazily). */
static void build_primes(void) {
    if (primes_built) return;
    primes_built = 1;
    static int is_c[PRIMES_LIMIT + 1] = {0};
    for (int i = 2; i <= PRIMES_LIMIT; ++i) {
        if (!is_c[i]) {
            primes[primes_cnt++] = i;
            if ((int64_t)i * i <= PRIMES_LIMIT)
                for (int j = i * i; j <= PRIMES_LIMIT; j += i)
                    is_c[j] = 1;
        }
    }
}

/* Möbius function for integer d (returns -1, 0, or +1). */
static int mu_of(idx_t d) {
    if (d == 1) return 1;
    int mu = 1;
    for (size_t p = 0; p < primes_cnt && (idx_t)primes[p] * primes[p] <= d; ++p) {
        int prime = primes[p];
        if (d % prime == 0) {
            d /= prime;
            if (d % prime == 0) return 0;  /* squared prime factor */
            mu = -mu;
            while (d % prime == 0) d /= prime;
        }
    }
    if (d > 1) mu = -mu;   /* remaining prime factor */
    return mu;
}

/* A027306(k) = 2^{k-1} + [k even] * C(k-1, k/2-1)   (OEIS) */
static seq_t A027306_of(idx_t k) {
    seq_t res = pow2(k - 1);          /* 2^{k-1} */
    if ((k & 1) == 0) {              /* k even */
        idx_t h = k / 2 - 1;          /* k/2 - 1 */
        res += binom(k - 1, h);       /* + C(k-1, k/2-1) */
    }
    return res;
}

/* Main sequence function. */
seq_t a(idx_t n) {
    if (n == 0) return 0;                /* offset is 1; unused below */
    build_primes();

    seq_t sum = 0;
    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i != 0) continue;
        /* first divisor d1 = i */
        idx_t d1 = i;
        int mu1 = mu_of(d1);
        if (mu1 != 0) {
            seq_t term = A027306_of(n / d1);
            if (mu1 == 1) sum += term; else sum -= term;
        }
        /* second divisor d2 = n / i, if distinct */
        idx_t d2 = n / i;
        if (d2 != d1) {
            int mu2 = mu_of(d2);
            if (mu2 != 0) {
                seq_t term = A027306_of(n / d2);
                if (mu2 == 1) sum += term; else sum -= term;
            }
        }
    }
    return sum / (seq_t)n;   /* division is exact */
}
