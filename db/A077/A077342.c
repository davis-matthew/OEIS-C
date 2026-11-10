/*
 * A077342  First column of A077341.
 * https://oeis.org/A077342
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = first term of row n of A077341, where row n holds the
 *            first n numbers not already used that begin with the digits of
 *            n; computed by direct search with a used-number bitset
 * Range:     u8 n <= 29, u16 n <= 659, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 83 terms through n = 9966 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static unsigned long long *A077342_used = NULL;
static const uint64_t A077342_UB = 200000000ULL;
static uint64_t *A077342_first = NULL;
static uint64_t A077342_cap = 0;
static uint64_t A077342_rows = 0;

static int A077342_test(uint64_t v) {
    if (v > A077342_UB) return 0;
    return (int)((A077342_used[v >> 6] >> (v & 63)) & 1ULL);
}

static void A077342_mark(uint64_t v) {
    if (v > A077342_UB) return;
    A077342_used[v >> 6] |= 1ULL << (v & 63);
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (!A077342_used) {
        size_t len = (size_t)(A077342_UB >> 6) + 1;
        A077342_used = (unsigned long long *)calloc(len, sizeof(unsigned long long));
        if (!A077342_used) exit(1);
    }
    if (n > A077342_cap) {
        uint64_t nc = A077342_cap ? A077342_cap : 256;
        while (nc < n) nc <<= 1;
        uint64_t *t = (uint64_t *)realloc(A077342_first, nc * sizeof(uint64_t));
        if (!t) exit(1);
        A077342_first = t;
        A077342_cap = nc;
    }
    while (A077342_rows < n) {
        uint64_t r = A077342_rows + 1;
        uint64_t needed = r;
        int have_first = 0;
        uint64_t first = 0;
        if (!A077342_test(r)) {
            A077342_mark(r);
            first = r;
            have_first = 1;
            --needed;
        }
        uint64_t p = 10;
        while (needed > 0) {
            uint64_t base = r * p;
            for (uint64_t off = 0; off < p && needed > 0; ++off) {
                uint64_t v = base + off;
                if (!A077342_test(v)) {
                    A077342_mark(v);
                    if (!have_first) { first = v; have_first = 1; }
                    --needed;
                }
            }
            p *= 10;
        }
        A077342_first[r - 1] = first;
        A077342_rows = r;
    }
    return (seq_t)A077342_first[n - 1];
}
