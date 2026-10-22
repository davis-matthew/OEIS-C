/*
 * A000921  Primes p of the form 3k+1 such that Sum_{x=1..p} cos(2*Pi*x^3/p) >
 *          sqrt(p).
 * https://oeis.org/A000921
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search over primes p == 1 (mod 3), keeping those with
 *            Sum_{x=1..p} cos(2*Pi*x^3/p) > sqrt(p). The sum is computed as 1
 *            + 2*Sum_{x=1..(p-1)/2} cos(2*Pi*(x^3 mod p)/p) using a
 *            rotation-built cosine table (all on native p, never on seq_t)
 * Range:     u8 n <= 12, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   a921_cossin, a921_isprime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/a921_cossin.h"
#include "../utils/a921_isprime.h"

static uint64_t *a921_terms = NULL;
static idx_t a921_cnt = 0, a921_cap = 0;
static uint64_t a921_next = 2;
static double *a921_cos = NULL, *a921_sin = NULL;
static uint64_t a921_tcap = 0;

static bool a921_test(uint64_t p)
{
    const double TWO_PI = 6.28318530717958647692528676655900577;
    uint64_t h = (p - 1) / 2, k, x;
    double c1, s1, S;
    if (h + 1 > a921_tcap) {
        free(a921_cos);
        free(a921_sin);
        a921_tcap = 2 * (h + 1);
        a921_cos = (double *)malloc(sizeof(double) * a921_tcap);
        a921_sin = (double *)malloc(sizeof(double) * a921_tcap);
    }
    a921_cossin(TWO_PI / (double)p, &c1, &s1);
    a921_cos[0] = 1.0;
    a921_sin[0] = 0.0;
    for (k = 0; k < h; k++) {
        a921_cos[k + 1] = a921_cos[k] * c1 - a921_sin[k] * s1;
        a921_sin[k + 1] = a921_sin[k] * c1 + a921_cos[k] * s1;
    }
    S = 0.0;
    for (x = 1; x <= h; x++) {
        uint64_t r = x * x % p * x % p;
        if (r > h) r = p - r;
        S += a921_cos[r];
    }
    S = 1.0 + 2.0 * S;
    return S > 0.0 && S * S > (double)p;
}

seq_t a(idx_t n)
{
    while (a921_cnt < n) {
        uint64_t p = a921_next++;
        if (p % 3 != 1 || !a921_isprime(p)) continue;
        if (a921_test(p)) {
            if (a921_cnt == a921_cap) {
                idx_t nc = a921_cap ? 2 * a921_cap : 1024;
                uint64_t *t = (uint64_t *)malloc(sizeof(uint64_t) * nc);
                idx_t i;
                for (i = 0; i < a921_cnt; i++) t[i] = a921_terms[i];
                free(a921_terms);
                a921_terms = t;
                a921_cap = nc;
            }
            a921_terms[a921_cnt++] = p;
        }
    }
    return (seq_t)a921_terms[n - 1];
}
