/*
 * A225047  Inverse permutation to A121216.
 * https://oeis.org/A225047
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = k such that A121216(k) = n (inverse permutation of
 *            A121216, where A121216(1)=1, A121216(2)=2, A121216(n)=smallest
 *            unused positive integer coprime to A121216(n-2)); computed by
 *            generating A121216 until n appears
 * Range:     u8 n <= 251, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 83 terms through n = 9934 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static uint64_t *A225047_vals = NULL;
static size_t A225047_len = 0, A225047_cap = 0;
static uint64_t *A225047_pos = NULL;   /* pos[v] = 1-based index of v, 0 if unseen */
static size_t A225047_pcap = 0;
static uint64_t A225047_lo = 1;        /* smallest possibly-unused value */

static uint64_t A225047_gcd(uint64_t x, uint64_t y)
{
    while (y) { uint64_t t = x % y; x = y; y = t; }
    return x;
}

static void A225047_ensure_pos(size_t idx)
{
    if (idx < A225047_pcap) return;
    size_t nc = A225047_pcap ? A225047_pcap : 16;
    while (nc <= idx) nc *= 2;
    uint64_t *t = (uint64_t *)realloc(A225047_pos, nc * sizeof(uint64_t));
    for (size_t i = A225047_pcap; i < nc; ++i) t[i] = 0;
    A225047_pos = t;
    A225047_pcap = nc;
}

static void A225047_push(uint64_t v)
{
    if (A225047_len == A225047_cap) {
        A225047_cap = A225047_cap ? A225047_cap * 2 : 16;
        A225047_vals = (uint64_t *)realloc(A225047_vals, A225047_cap * sizeof(uint64_t));
    }
    A225047_vals[A225047_len++] = v;
    A225047_ensure_pos((size_t)v);
    A225047_pos[(size_t)v] = (uint64_t)A225047_len;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (A225047_len == 0) {
        A225047_ensure_pos(16);
        A225047_push(1);
        A225047_push(2);
        A225047_lo = 3;
    }
    for (;;) {
        if ((size_t)n < A225047_pcap && A225047_pos[(size_t)n] != 0)
            return (seq_t)A225047_pos[(size_t)n];
        uint64_t pp = A225047_vals[A225047_len - 2];
        for (;;) {
            A225047_ensure_pos((size_t)A225047_lo);
            if (A225047_pos[(size_t)A225047_lo] == 0) break;
            ++A225047_lo;
        }
        uint64_t c = A225047_lo;
        for (;;) {
            A225047_ensure_pos((size_t)c);
            if (A225047_pos[(size_t)c] == 0 && A225047_gcd(c, pp) == 1) break;
            ++c;
        }
        A225047_push(c);
    }
}
