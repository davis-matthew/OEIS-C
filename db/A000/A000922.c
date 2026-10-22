/*
 * A000922  Primes p of the form 3k+1 such that -sqrt(p) < Sum_{x=1..p}
 *          cos(2*Pi*x^3/p) < sqrt(p).
 * https://oeis.org/A000922
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search over primes p = 1 (mod 3), keeping p when S(p)^2 < p,
 *            where S(p) = Sum_{x=1..p} cos(2*Pi*x^3/p) = 1 +
 *            2*Sum_{x=1..(p-1)/2} cos(2*Pi*(x^3 mod p)/p). S(p) is a root of
 *            t^3 - 3*p*t - p*L (4p = L^2 + 27M^2), so it is never within
 *            about 1.5 of +-sqrt(p); a cosine table with linear
 *            interpolation...
 * Range:     u8 n <= 8, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   a922_isprime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/a922_isprime.h"

#define A922_TB 16384

static double a922_tab[A922_TB + 1];
static int a922_tab_ready = 0;

static void a922_init_tab(void) {
    const double pi = 3.14159265358979323846;
    double h = 2.0 * pi / (double)A922_TB;
    double h2 = h * h;
    double ch = 1.0 - h2 / 2.0 + h2 * h2 / 24.0 - h2 * h2 * h2 / 720.0;
    double sh = h * (1.0 - h2 / 6.0 + h2 * h2 / 120.0 - h2 * h2 * h2 / 5040.0);
    double cr = 1.0, ci = 0.0;
    int i;
    for (i = 0; i <= A922_TB; i++) {
        double nr, ni;
        a922_tab[i] = cr;
        nr = cr * ch - ci * sh;
        ni = cr * sh + ci * ch;
        cr = nr;
        ci = ni;
    }
    a922_tab_ready = 1;
}

static bool a922_ok(uint64_t p) {
    uint64_t c = 1, d = 7 % p, e = 12 % p, x, half = (p - 1) / 2;
    double scale = (double)A922_TB / (double)p;
    double sum = 0.0, S;
    for (x = 1; x <= half; x++) {
        double pos = (double)c * scale;
        int i = (int)pos;
        double f = pos - (double)i;
        sum += a922_tab[i] + (a922_tab[i + 1] - a922_tab[i]) * f;
        c += d; if (c >= p) c -= p;
        d += e; if (d >= p) d -= p;
        e += 6; if (e >= p) e -= p;
    }
    S = 1.0 + 2.0 * sum;
    return S * S < (double)p;
}

static uint64_t *a922_terms = NULL;
static idx_t a922_cnt = 0, a922_cap = 0;
static uint64_t a922_next_p = 7;

seq_t a(idx_t n) {
    if (!a922_tab_ready) a922_init_tab();
    while (a922_cnt < n) {
        uint64_t p = a922_next_p;
        a922_next_p += 6;
        if (!a922_isprime(p)) continue;
        if (!a922_ok(p)) continue;
        if (a922_cnt == a922_cap) {
            idx_t nc = a922_cap ? a922_cap * 2 : 256;
            a922_terms = (uint64_t *)realloc(a922_terms, (size_t)nc * sizeof(uint64_t));
            a922_cap = nc;
        }
        a922_terms[a922_cnt++] = p;
    }
    return (seq_t)a922_terms[n - 1];
}
