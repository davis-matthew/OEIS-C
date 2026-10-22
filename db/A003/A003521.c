/*
 * A003521  Values of m in the discriminant D = -4*m leading to a new minimum
 *          of the L-function of the Dirichlet series L(1) = Sum_{k>=1}
 *          Kronecker(D,k)/k.
 * https://oeis.org/A003521
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   search. For m >= 2, L(1) for D = -4m equals
 *            Pi*h(-4m)/(2*sqrt(m)), where h is the class number of primitive
 *            forms of discriminant -4m (for m = 1, w = 4 gives L = Pi/4). A
 *            truncated Euler product over primes < 1000 picks out candidate
 *            m. For each candidate, h(-4m) is found exactly by counting
 *            redu...
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 14, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   a3521_classno, a3521_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include "../utils/a3521_classno.h"
#include "../utils/a3521_gcd.h"

#define A3521_PMAX 1000

static int s_np = 0;
static int s_pr[200];
static unsigned char *s_chi[200];
static double s_lw[200][3];
static int s_res[200];
static int s_init = 0;

static uint64_t *s_found = NULL;
static idx_t s_cnt = 0, s_cap = 0;
static uint64_t s_m = 1;
static uint64_t s_bestH = 1, s_bestm = 1; /* 4*h^2 and m of current record; m=1 -> 4*(1/2)^2 = 1 */
static double s_minL = 0.0;

static void a3521_push(uint64_t v) {
    if (s_cnt == s_cap) {
        s_cap = s_cap ? s_cap * 2 : 64;
        s_found = (uint64_t *)realloc(s_found, (size_t)s_cap * sizeof(uint64_t));
    }
    s_found[s_cnt++] = v;
}

static void a3521_setup(void) {
    unsigned char sieve[A3521_PMAX];
    int i, x;
    for (i = 0; i < A3521_PMAX; i++) sieve[i] = 1;
    for (i = 2; i < A3521_PMAX; i++) {
        if (!sieve[i]) continue;
        for (x = i * i; x < A3521_PMAX; x += i) sieve[x] = 0;
        if (i == 2) continue;
        {
            int p = i, r, k;
            unsigned char *sq = (unsigned char *)calloc((size_t)p, 1);
            for (x = 1; x < p; x++) sq[(x * x) % p] = 1;
            s_chi[s_np] = (unsigned char *)malloc((size_t)p);
            for (r = 0; r < p; r++) {
                int t = (p - r) % p; /* chi_{-4m}(p) = (-m / p) */
                s_chi[s_np][r] = (unsigned char)(t == 0 ? 1 : (sq[t] ? 2 : 0));
            }
            free(sq);
            for (k = 0; k < 3; k++)
                s_lw[s_np][k] = -log(1.0 - (double)(k - 1) / (double)p);
            s_pr[s_np] = p;
            s_res[s_np] = 1 % p;
            s_np++;
        }
    }
    s_minL = 3.14159265358979323846 / 4.0;
    a3521_push(1);
    s_init = 1;
}

seq_t a(idx_t n) {
    if (!s_init) a3521_setup();
    while (s_cnt < n) {
        int i;
        double s = 0.0;
        s_m++;
        for (i = 0; i < s_np; i++) {
            int r = s_res[i] + 1;
            if (r == s_pr[i]) r = 0;
            s_res[i] = r;
            s += s_lw[i][s_chi[i][r]];
        }
        if (s < log(1.15 * s_minL)) {
            uint64_t h = a3521_classno(s_m);
            uint64_t H = 4 * h * h;
            if ((unsigned __int128)H * s_bestm < (unsigned __int128)s_bestH * s_m) {
                s_bestH = H;
                s_bestm = s_m;
                s_minL = 3.14159265358979323846 * (double)h / (2.0 * sqrt((double)s_m));
                a3521_push(s_m);
            }
        }
    }
    return (seq_t)s_found[n - 1];
}
