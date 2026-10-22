/*
 * A000923  Primes p of the form 3k+1 such that Sum_{x=1..p} cos(2*Pi*x^3/p) <
 *          -sqrt(p).
 * https://oeis.org/A000923
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search over primes p == 1 (mod 3), keeping those where S(p) =
 *            Sum_{x=1..p} cos(2*Pi*x^3/p) < -sqrt(p). S(p) = 1 +
 *            2*Sum_{x=1..(p-1)/2} cos(2*Pi*(x^3 mod p)/p): a cosine table is
 *            built by rotation and x^3 mod p is updated by finite differences
 * Range:     u8 n <= 5, u16 n <= 805, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   a923_isprime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/a923_isprime.h"

static const double A923_TWO_PI = 6.283185307179586476925286766559;

static double *a923_tab = NULL;
static uint64_t a923_tabcap = 0;

static double a923_sum(uint64_t p) {
    uint64_t half = (p - 1) / 2;
    if (half + 1 > a923_tabcap) {
        free(a923_tab);
        a923_tabcap = 2 * (half + 1);
        a923_tab = (double *)malloc(a923_tabcap * sizeof(double));
    }
    double c1 = cos(A923_TWO_PI / (double)p), s1 = sin(A923_TWO_PI / (double)p);
    double c = 1.0, s = 0.0;
    for (uint64_t k = 0; k <= half; k++) {
        if ((k & 511) == 0) {
            double ang = A923_TWO_PI * (double)k / (double)p;
            c = cos(ang);
            s = sin(ang);
        } else {
            double nc = c * c1 - s * s1;
            s = s * c1 + c * s1;
            c = nc;
        }
        a923_tab[k] = c;
    }
    uint64_t c3 = 0, d = 1, e = 6 % p;
    double sum = 0.0;
    for (uint64_t x = 1; x <= half; x++) {
        c3 += d; if (c3 >= p) c3 -= p;
        d += e;  if (d >= p) d -= p;
        e += 6;  if (e >= p) e -= p;
        uint64_t r = (c3 <= half) ? c3 : p - c3;
        sum += a923_tab[r];
    }
    return 1.0 + 2.0 * sum;
}

static uint64_t *a923_terms = NULL;
static uint64_t a923_cnt = 0, a923_cap = 0;
static uint64_t a923_next = 7;

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;
    while (a923_cnt < n) {
        uint64_t p = a923_next;
        a923_next += 6;
        if (!a923_isprime(p)) continue;
        double S = a923_sum(p);
        if (S < -sqrt((double)p)) {
            if (a923_cnt == a923_cap) {
                a923_cap = a923_cap ? 2 * a923_cap : 1024;
                a923_terms = (uint64_t *)realloc(a923_terms, a923_cap * sizeof(uint64_t));
            }
            a923_terms[a923_cnt++] = p;
        }
    }
    return (seq_t)a923_terms[n - 1];
}
