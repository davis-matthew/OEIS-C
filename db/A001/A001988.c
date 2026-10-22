/*
 * A001988  Let p be the n-th odd prime. a(n) is the least prime congruent to
 *          7 modulo 8 such that Legendre(-a(n), q) = -Legendre(-1, q) for all
 *          odd primes q <= p.
 * https://oeis.org/A001988
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 24, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   a1988_isprime, a1988_mulmod, a1988_powmod, a1988_wcmp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) = least prime c == 7 (mod 8) with Legendre(-c, q) = -1 for all
   odd primes q <= p_n, the n-th odd prime. (This matches the listed terms.)
   The candidates are scanned in increasing order with a CRT wheel modulo
   8*3*5*7*11*13*17*19*23, then tested against more primes q and checked for
   primality. Found terms are memoized because the sequence is nondecreasing. */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/a1988_isprime.h"
#include "../utils/a1988_mulmod.h"
#include "../utils/a1988_powmod.h"
#include "../utils/a1988_wcmp.h"

#define A1988_NTAB 200
#define A1988_MAXT 20000

static uint32_t *a1988_pr = NULL;
static int a1988_npr = 0;
static unsigned char **a1988_nr = NULL;
static uint64_t a1988_terms[A1988_MAXT + 2];
static int a1988_filled = 0;
static uint64_t a1988_brute = 7;
static uint64_t *a1988_res = NULL;
static uint64_t a1988_nres = 0, a1988_M = 0, a1988_blk = 0, a1988_idx = 0;

static void a1988_init(void) {
    uint32_t lim = 300000, i, j;
    char *sv;
    int k;
    if (a1988_pr) return;
    sv = (char *)calloc(lim + 1, 1);
    a1988_pr = (uint32_t *)malloc(sizeof(uint32_t) * (lim / 2 + 1));
    for (i = 2; i <= lim; i++) {
        if (sv[i]) continue;
        if (i > 2) a1988_pr[a1988_npr++] = i;
        for (j = i * 2; j <= lim; j += i) sv[j] = 1;
    }
    free(sv);
    a1988_nr = (unsigned char **)malloc(sizeof(unsigned char *) * A1988_NTAB);
    for (k = 0; k < A1988_NTAB; k++) {
        uint32_t q = a1988_pr[k], y;
        unsigned char *t = (unsigned char *)malloc(q);
        memset(t, 1, q);
        t[0] = 0;
        for (y = 1; y < q; y++) t[(uint64_t)y * y % q] = 0;
        a1988_nr[k] = t;
    }
}

/* is -c a quadratic nonresidue mod the i-th odd prime? */
static int a1988_ok(uint64_t c, int i) {
    uint64_t q = a1988_pr[i];
    uint64_t x = (q - c % q) % q;
    if (i < A1988_NTAB) return a1988_nr[i][x];
    if (x == 0) return 0;
    return a1988_powmod(x, (q - 1) / 2, q) == q - 1;
}

static int a1988_count(uint64_t c) {
    int i = 0;
    while (i < a1988_npr && i < A1988_MAXT && a1988_ok(c, i)) i++;
    return i;
}

static void a1988_build_wheel(void) {
    uint64_t *cur, *nxt, cnt = 1, m = 8, r, t, nc;
    int i;
    cur = (uint64_t *)malloc(sizeof(uint64_t));
    cur[0] = 7;
    for (i = 0; i < 8; i++) {
        uint64_t q = a1988_pr[i], j;
        nxt = (uint64_t *)malloc(sizeof(uint64_t) * cnt * q);
        nc = 0;
        for (j = 0; j < cnt; j++) {
            for (t = 0; t < q; t++) {
                r = cur[j] + t * m;
                if (a1988_nr[i][(q - r % q) % q]) nxt[nc++] = r;
            }
        }
        free(cur);
        cur = nxt;
        cnt = nc;
        m *= q;
    }
    qsort(cur, cnt, sizeof(uint64_t), a1988_wcmp);
    a1988_res = cur;
    a1988_nres = cnt;
    a1988_M = m;
}

static void a1988_fill(uint64_t c, int k) {
    int m;
    if (k > A1988_MAXT) k = A1988_MAXT;
    for (m = a1988_filled + 1; m <= k; m++) a1988_terms[m] = c;
    if (k > a1988_filled) a1988_filled = k;
}

seq_t a(idx_t n) {
    int need;
    uint64_t last;
    a1988_init();
    if (n > A1988_MAXT) n = A1988_MAXT;
    need = (int)n;
    while (a1988_filled < need && a1988_filled < 8) {
        uint64_t c = a1988_brute;
        int k;
        a1988_brute += 8;
        k = a1988_count(c);
        if (k > a1988_filled && a1988_isprime(c)) a1988_fill(c, k);
    }
    if (a1988_filled < need) {
        if (!a1988_res) a1988_build_wheel();
        last = a1988_brute - 8;
        while (a1988_filled < need) {
            uint64_t c = a1988_blk * a1988_M + a1988_res[a1988_idx];
            int k;
            a1988_idx++;
            if (a1988_idx == a1988_nres) { a1988_idx = 0; a1988_blk++; }
            if (c <= last) continue;
            k = a1988_count(c);
            if (k > a1988_filled && a1988_isprime(c)) a1988_fill(c, k);
        }
    }
    return (seq_t)a1988_terms[need];
}
