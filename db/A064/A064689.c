/*
 * A064689  Smallest m such that A064672(m) = n.
 * https://oeis.org/A064689
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = smallest m with A064672(m) = n, where A064672(0)=0,
 *            A064672(1)=1, A064672(k) = A064672(x) + A064672(k - x^2), x =
 *            floor(sqrt(k)); found by incremental search
 * Range:     u8 n <= 14, u16 n <= 28, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Validated: partial, pass, 36 terms through n = 35 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

#define A064689_CAP 1024

static uint64_t *A064689_f = NULL;
static uint64_t A064689_cap = 0;
static uint64_t A064689_cnt = 0;   /* number of f values computed */
static uint64_t A064689_x = 1;     /* floor(sqrt(next m)) */
static uint64_t A064689_first[A064689_CAP];
static unsigned char A064689_found[A064689_CAP];

static void A064689_record(uint64_t m, uint64_t v) {
    if (v < A064689_CAP && !A064689_found[v]) {
        A064689_found[v] = 1;
        A064689_first[v] = m;
    }
}

static void A064689_step(void) {
    if (A064689_cnt >= A064689_cap) {
        uint64_t nc = A064689_cap ? A064689_cap * 2 : 1024;
        uint64_t *p = (uint64_t *)realloc(A064689_f, nc * sizeof(uint64_t));
        if (!p) abort();
        A064689_f = p;
        A064689_cap = nc;
    }
    uint64_t m = A064689_cnt;
    uint64_t v;
    if (m == 0) v = 0;
    else if (m == 1) v = 1;
    else {
        while ((A064689_x + 1) * (A064689_x + 1) <= m) A064689_x++;
        v = A064689_f[A064689_x] + A064689_f[m - A064689_x * A064689_x];
    }
    A064689_f[m] = v;
    A064689_cnt++;
    A064689_record(m, v);
}

seq_t a(idx_t n) {
    if (n >= A064689_CAP) return 0;
    while (!A064689_found[n]) A064689_step();
    return (seq_t)A064689_first[n];
}
