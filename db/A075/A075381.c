/*
 * A075381  Fixed points of A075380.
 * https://oeis.org/A075381
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k such that A075380(k) = k, where A075380 is the
 *            greedy permutation of the naturals with every neighbouring sum
 *            non-squarefree (search)
 * Range:     u8 n <= 37, u16 n <= 8933, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 71 terms through n = 9038 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

static uint8_t *A075381_used = NULL;
static size_t A075381_used_cap = 0;
static uint8_t *A075381_sf = NULL;
static size_t A075381_sf_cap = 0;
static uint64_t *A075381_fp = NULL;
static size_t A075381_fp_n = 0, A075381_fp_cap = 0;
static uint64_t A075381_idx = 0, A075381_prev = 0, A075381_lo = 1;

static void A075381_ensure_used(size_t need)
{
    if (need < A075381_used_cap) return;
    size_t nc = A075381_used_cap ? A075381_used_cap * 2 : 1024;
    while (nc <= need) nc *= 2;
    A075381_used = (uint8_t *)realloc(A075381_used, nc);
    memset(A075381_used + A075381_used_cap, 0, nc - A075381_used_cap);
    A075381_used_cap = nc;
}

static void A075381_ensure_sf(size_t need)
{
    if (need < A075381_sf_cap) return;
    size_t nc = A075381_sf_cap ? A075381_sf_cap * 2 : 4096;
    while (nc <= need) nc *= 2;
    free(A075381_sf);
    A075381_sf = (uint8_t *)malloc(nc);
    memset(A075381_sf, 1, nc);
    for (size_t k = 2; k * k < nc; ++k) {
        size_t q = k * k;
        for (size_t j = q; j < nc; j += q) A075381_sf[j] = 0;
    }
    A075381_sf_cap = nc;
}

uint64_t A075381_dummy_unused;

seq_t a(idx_t n)
{
    while (A075381_fp_n < (size_t)n) {
        uint64_t cand;
        A075381_idx++;
        if (A075381_idx == 1) {
            cand = 1;
        } else {
            cand = A075381_lo;
            for (;;) {
                A075381_ensure_used((size_t)cand);
                if (A075381_used[cand]) { ++cand; continue; }
                size_t s = (size_t)(A075381_prev + cand);
                A075381_ensure_sf(s);
                if (!A075381_sf[s]) break;
                ++cand;
            }
        }
        A075381_ensure_used((size_t)cand);
        A075381_used[cand] = 1;
        while (A075381_used[A075381_lo]) {
            A075381_lo++;
            A075381_ensure_used((size_t)A075381_lo);
        }
        A075381_prev = cand;
        if (cand == A075381_idx) {
            if (A075381_fp_n == A075381_fp_cap) {
                A075381_fp_cap = A075381_fp_cap ? A075381_fp_cap * 2 : 64;
                A075381_fp = (uint64_t *)realloc(A075381_fp, A075381_fp_cap * sizeof(uint64_t));
            }
            A075381_fp[A075381_fp_n++] = cand;
        }
    }
    return (seq_t)A075381_fp[(size_t)n - 1];
}
