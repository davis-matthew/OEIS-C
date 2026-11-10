/*
 * A229167  a(n) = smallest index i such that A010062(i) >= 2^n.
 * https://oeis.org/A229167
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of steps of x -> x + A000120(x), starting from x =
 *            1, until x >= 2^n (a(0) = 0). Computed by divide and conquer on
 *            the bit window: steps(p, k, L) counts the steps from a k-bit low
 *            part L, with p one-bits above it, until the low part carries
 *            out. It splits on bit k-1 and is memoized
 * Range:     u8 n <= 10, u16 n <= 19, u32 n <= 36, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Validated: standard, pass, 47 terms through n = 46 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

static uint64_t *A229167_hkey = NULL;
static seq_t *A229167_hsteps = NULL;
static uint64_t *A229167_hr = NULL;
static size_t A229167_hcap = 0, A229167_hcnt = 0;

static unsigned A229167_pc64(uint64_t x) {
    unsigned c = 0;
    while (x) { c += (unsigned)(x & 1u); x >>= 1; }
    return c;
}

static size_t A229167_slot(uint64_t key) {
    size_t mask = A229167_hcap - 1;
    size_t h = (size_t)((key * 0x9E3779B97F4A7C15ull) >> 17) & mask;
    while (A229167_hkey[h] != 0 && A229167_hkey[h] != key) h = (h + 1) & mask;
    return h;
}

static void A229167_grow(void) {
    size_t oldcap = A229167_hcap;
    uint64_t *ok = A229167_hkey;
    seq_t *os = A229167_hsteps;
    uint64_t *orr = A229167_hr;
    size_t ncap = oldcap ? oldcap * 2 : 4096;
    A229167_hkey = (uint64_t *)calloc(ncap, sizeof(uint64_t));
    A229167_hr = (uint64_t *)calloc(ncap, sizeof(uint64_t));
    A229167_hsteps = mem_new(seq_t, ncap);
    A229167_hcap = ncap;
    for (size_t i = 0; i < oldcap; ++i) {
        if (ok[i]) {
            size_t s = A229167_slot(ok[i]);
            A229167_hkey[s] = ok[i];
            A229167_hsteps[s] = os[i];
            A229167_hr[s] = orr[i];
        }
    }
    if (ok) { free(ok); free(orr); mem_free(os); }
}

/* Window of k bits with value L, p one-bits above it; each step adds p + popcount(L).
   Returns the number of steps until L >= 2^k, and r = L - 2^k at that point. */
static void A229167_go(uint64_t p, unsigned k, uint64_t L, seq_t *steps, uint64_t *r) {
    if (k <= 6 || (k < 63 && (1ull << (k - 1)) <= p + k)) {
        uint64_t lim = 1ull << k;
        uint64_t cnt = 0;
        while (L < lim) {
            L += p + A229167_pc64(L);
            ++cnt;
        }
        *steps = (seq_t)cnt;
        *r = L - lim;
        return;
    }
    int memo = (L < (1ull << 24)) && (p < (1ull << 19)) && (k < (1u << 20));
    uint64_t key = 0;
    if (memo) {
        if (A229167_hcap == 0) A229167_grow();
        key = ((p + 1) << 44) | ((uint64_t)k << 24) | L;
        size_t s = A229167_slot(key);
        if (A229167_hkey[s] == key) {
            *steps = A229167_hsteps[s];
            *r = A229167_hr[s];
            return;
        }
    }
    seq_t res;
    uint64_t rr;
    int topset = (k - 1 < 64) && (((L >> (k - 1)) & 1u) != 0);
    if (topset) {
        A229167_go(p + 1, k - 1, L - (1ull << (k - 1)), &res, &rr);
    } else {
        seq_t s1, s2;
        uint64_t r1, r2;
        A229167_go(p, k - 1, L, &s1, &r1);
        A229167_go(p + 1, k - 1, r1, &s2, &r2);
        res = s1 + s2;
        rr = r2;
    }
    if (memo) {
        if ((A229167_hcnt + 1) * 2 >= A229167_hcap) A229167_grow();
        size_t s = A229167_slot(key);
        if (A229167_hkey[s] != key) {
            A229167_hkey[s] = key;
            A229167_hcnt++;
        }
        A229167_hsteps[s] = res;
        A229167_hr[s] = rr;
    }
    *steps = res;
    *r = rr;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    seq_t steps;
    uint64_t r;
    A229167_go(0, (unsigned)n, 1, &steps, &r);
    return steps;
}
