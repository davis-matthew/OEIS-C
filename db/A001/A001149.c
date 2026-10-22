/*
 * A001149  A self-generating sequence: a(1)=1, a(2)=2, a(n+1) chosen so that
 *          a(n+1)-a(n-1) is the first number not obtainable as a(j)-a(i) for
 *          1<=i<j<=n.
 * https://oeis.org/A001149
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   computed by direct simulation: a(1)=1, a(2)=2, a(n+1) = a(n-1) +
 *            d, where d is the smallest positive integer not yet occurring as
 *            a difference a(j)-a(i), 1<=i<j<=n (all differences seen so far
 *            are kept in a bitmap)
 * Range:     u8 n <= 20, u16 n <= 262, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static uint64_t *a1149_vals = NULL;
static idx_t a1149_cap = 0;
static idx_t a1149_done = 0;
static unsigned char *a1149_bits = NULL;
static uint64_t a1149_bitcap = 0; /* number of bits available */
static uint64_t a1149_mex = 1;

static void a1149_ensure_bits(uint64_t need) {
    if (need < a1149_bitcap) return;
    uint64_t nc = a1149_bitcap ? a1149_bitcap : 1024;
    while (nc <= need) nc *= 2;
    size_t oldb = (size_t)(a1149_bitcap / 8);
    size_t newb = (size_t)(nc / 8);
    a1149_bits = (unsigned char *)realloc(a1149_bits, newb);
    memset(a1149_bits + oldb, 0, newb - oldb);
    a1149_bitcap = nc;
}

static void a1149_mark(uint64_t d) {
    a1149_bits[d >> 3] |= (unsigned char)(1u << (d & 7));
}

static int a1149_test(uint64_t d) {
    return (a1149_bits[d >> 3] >> (d & 7)) & 1;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    if (a1149_done == 0) {
        a1149_cap = 1024;
        a1149_vals = (uint64_t *)malloc((size_t)a1149_cap * sizeof(uint64_t));
        a1149_vals[0] = 0;
        a1149_vals[1] = 1;
        a1149_vals[2] = 2;
        a1149_ensure_bits(16);
        a1149_mark(1);
        a1149_mex = 1;
        while (a1149_test(a1149_mex)) a1149_mex++;
        a1149_done = 2;
    }
    if (n >= a1149_cap) {
        idx_t nc = a1149_cap;
        while (nc <= n) nc *= 2;
        a1149_vals = (uint64_t *)realloc(a1149_vals, (size_t)nc * sizeof(uint64_t));
        a1149_cap = nc;
    }
    while (a1149_done < n) {
        idx_t k = a1149_done + 1;
        uint64_t v = a1149_vals[k - 2] + a1149_mex;
        a1149_vals[k] = v;
        a1149_ensure_bits(v + 2);
        for (idx_t i = 1; i < k; i++) {
            if (v > a1149_vals[i]) a1149_mark(v - a1149_vals[i]);
        }
        while (a1149_test(a1149_mex)) {
            a1149_mex++;
            a1149_ensure_bits(a1149_mex + 1);
        }
        a1149_done = k;
    }
    return (seq_t)a1149_vals[n];
}
