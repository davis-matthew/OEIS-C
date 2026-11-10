/*
 * A207506  Indices for which A207505(n) = 0.
 * https://oeis.org/A207506
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th (0-indexed) k >= 0 with A207505(k) = 0; found by
 *            direct simulation of the digit-subtraction process, searching k
 *            upward
 * Range:     u8 n <= 64, u16 n <= 2167, u32 n <= 2167, u64 n <= 2167,
 *            u128 n <= 2167, big any n
 * Validated: standard, pass, 93 terms through n = 2166 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

static uint64_t *A207506_found = NULL;
static size_t A207506_cnt = 0, A207506_cap = 0;
static uint64_t A207506_next = 0;

static uint64_t *A207506_hist = NULL;
static size_t A207506_hcap = 0;

static size_t A207506_load(uint64_t x, uint8_t *dig)
{
    uint8_t tmp[24];
    size_t t = 0, i;
    if (x == 0) { dig[0] = 0; return 1; }
    while (x) { tmp[t++] = (uint8_t)(x % 10); x /= 10; }
    for (i = 0; i < t; i++) dig[i] = tmp[t - 1 - i];
    return t;
}

/* true iff A207505(n) == 0 */
static bool A207506_zero(uint64_t n)
{
    uint64_t v = n;
    size_t hc = 1, rj = 0, dn, dp = 0;
    uint8_t dig[24];
    if (v == 0) return true;
    if (A207506_hcap < 16) {
        A207506_hcap = 1024;
        free(A207506_hist);
        A207506_hist = (uint64_t *)malloc(A207506_hcap * sizeof(uint64_t));
        if (!A207506_hist) { A207506_hcap = 0; return false; }
    }
    A207506_hist[0] = n;
    dn = A207506_load(n, dig);
    while (v > 0) {
        uint64_t d;
        if (dp == dn) {
            rj++;
            dn = A207506_load(A207506_hist[rj], dig);
            dp = 0;
        }
        d = dig[dp++];
        if (v <= d) return v == d;
        v -= d;
        if (hc >= A207506_hcap) {
            size_t nc = A207506_hcap * 2;
            uint64_t *p = (uint64_t *)realloc(A207506_hist, nc * sizeof(uint64_t));
            if (!p) return false;
            A207506_hist = p;
            A207506_hcap = nc;
        }
        A207506_hist[hc++] = v;
    }
    return true;
}

seq_t a(idx_t n)
{
    while ((uint64_t)A207506_cnt <= (uint64_t)n) {
        if (A207506_zero(A207506_next)) {
            if (A207506_cnt >= A207506_cap) {
                size_t nc = A207506_cap ? A207506_cap * 2 : 1024;
                uint64_t *p = (uint64_t *)realloc(A207506_found, nc * sizeof(uint64_t));
                if (!p) return (seq_t)0;
                A207506_found = p;
                A207506_cap = nc;
            }
            A207506_found[A207506_cnt++] = A207506_next;
        }
        A207506_next++;
    }
    return (seq_t)A207506_found[(size_t)n];
}
