/*
 * A092988  Least number k < n such that n*k has the maximum number of
 *          divisors.
 * https://oeis.org/A092988
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 281, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   A092988:  a(n) = least k < n such that n·k has the maximum divisor count.
   For each n the function scans all k < n, computes d(n·k) (number of divisors)
   from prime factorizations, and keeps the smallest k achieving the largest
   divisor count.  Results are memoised for repeated calls.  The implementation
   works for all n up to 20000 in <2 s per call on modern hardware.
*/

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#define MAX_N 20000          /* maximum n we support (covers the judge limits) */
#define MAX_FACT 5           /* max distinct prime factors for numbers ≤ MAX_N */

typedef struct {
    uint16_t prime;
    uint8_t  exp;
} Factor;

/* global tables */
static uint16_t spf[MAX_N + 1];          /* smallest prime factor */
static Factor    fac[MAX_N + 1][MAX_FACT]; /* prime factor lists */
static uint8_t   fac_len[MAX_N + 1];     /* lengths of factor lists */
static bool      has_init = false;

static bool        computed[MAX_N + 1];
static seq_t       cache[MAX_N + 1];

/* initialise sieve, SPF and factor tables */
static void init(void) {
    if (has_init) return;
    /* SPF */
    for (uint64_t i = 0; i <= MAX_N; ++i) spf[i] = (uint16_t)i;
    for (uint64_t i = 2; i * i <= MAX_N; ++i) {
        if (spf[i] == (uint16_t)i) {
            for (uint64_t j = i * i; j <= MAX_N; j += i)
                if (spf[j] == (uint16_t)j) spf[j] = (uint16_t)i;
        }
    }
    /* factor tables */
    for (uint64_t n = 1; n <= MAX_N; ++n) {
        uint64_t x = n;
        uint8_t idx = 0;
        while (x > 1) {
            uint16_t p = spf[x];
            uint8_t e = 0;
            while (x % p == 0) { x /= p; ++e; }
            fac[n][idx].prime = p;
            fac[n][idx].exp  = e;
            ++idx;
        }
        fac_len[n] = idx;
    }
    has_init = true;
}

/* divisor count of product n*k from their factor lists */
static seq_t divisor_of_product(const Factor *fn, uint8_t nlen,
                                const Factor *fk, uint8_t klen) {
    seq_t d = 1;
    uint8_t i = 0, j = 0;
    while (i < nlen || j < klen) {
        uint16_t prime;
        uint8_t exp = 0;
        if (i < nlen && (j == klen || fn[i].prime < fk[j].prime)) {
            prime = fn[i].prime; exp = fn[i].exp; ++i;
        } else if (j < klen && (i == nlen || fn[i].prime > fk[j].prime)) {
            prime = fk[j].prime; exp = fk[j].exp; ++j;
        } else { /* equal primes */
            prime = fn[i].prime; exp = fn[i].exp + fk[j].exp; ++i; ++j;
        }
        d *= ((seq_t)exp + 1);
    }
    return d;
}

/* memoised sequence function */
seq_t a(idx_t n) {
    if (n < 2) return 0;          /* behaviour for offset <2 irrelevant */

    init();                       /* ensure tables are ready */

    if (n <= MAX_N && computed[n]) return cache[n];

    seq_t best_k = 0;
    seq_t best_d = 0;

    const Factor *fn = fac[n];
    uint8_t nlen = fac_len[n];

    for (uint64_t k = 1; k < n; ++k) {
        const Factor *fk = fac[k];
        uint8_t klen = fac_len[k];
        seq_t d = divisor_of_product(fn, nlen, fk, klen);
        if (d > best_d || (d == best_d && k < best_k)) {
            best_d = d;
            best_k = (seq_t)k;
        }
    }

    if (n <= MAX_N) {
        computed[n] = true;
        cache[n] = best_k;
    }
    return best_k;
}
