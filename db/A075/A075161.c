/*
 * A075161  Position of A075165(n+1) in A014486.
 * https://oeis.org/A075161
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = position in A014486 of the Dyck word F(n+1), where F(1) =
 *            empty word and, for m > 1 with largest prime p_k having exponent
 *            e and r = m/p_k^e, F(m) = "1" F(e) "0" F(r*p_{k-1}) (p_0 = 1).
 *            The position is sum_{t<s} Catalan(t) plus the rank among Dyck
 *            words of the same size s, computed by a ba...
 * Range:     u8 n <= 17, u16 n <= 35, u32 n <= 71, u64 n <= 161,
 *            u128 n <= 347, big any n
 * Validated: standard, pass, 73 terms through n = 1024 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static char *wd_buf = NULL;
static size_t wd_len = 0, wd_cap = 0;

static void wd_emit(char c)
{
    if (wd_len + 1 > wd_cap) {
        size_t nc = wd_cap ? wd_cap * 2 : 64;
        char *p = (char *)realloc(wd_buf, nc);
        if (!p) return;
        wd_buf = p;
        wd_cap = nc;
    }
    wd_buf[wd_len++] = c;
}

static bool wd_isprime(uint64_t q)
{
    if (q < 2) return false;
    if (q % 2 == 0) return q == 2;
    for (uint64_t d = 3; d * d <= q; d += 2)
        if (q % d == 0) return false;
    return true;
}

static uint64_t wd_prevprime(uint64_t p)
{
    uint64_t q = p - 1;
    while (q >= 2 && !wd_isprime(q)) q--;
    return q < 2 ? 1 : q;
}

static void wd_build(uint64_t m)
{
    while (m > 1) {
        uint64_t P = 1, e = 0, rem = m, r;
        for (uint64_t d = 2; d * d <= rem; d += (d == 2 ? 1 : 2)) {
            if (rem % d == 0) {
                uint64_t c = 0;
                while (rem % d == 0) { rem /= d; c++; }
                P = d; e = c;
            }
        }
        if (rem > 1) { P = rem; e = 1; }
        r = m;
        for (uint64_t i = 0; i < e; i++) r /= P;
        wd_emit('1');
        wd_build(e);
        wd_emit('0');
        m = r * wd_prevprime(P);
    }
}

seq_t a(idx_t n)
{
    wd_len = 0;
    wd_build((uint64_t)n + 1);
    size_t L = wd_len;
    if (L == 0) return (seq_t)0;
    size_t S = L / 2;
    size_t Wd = S + 2;
    seq_t *W = mem_new(seq_t, (L + 1) * Wd);
    W[0] = (seq_t)1;
    for (size_t r = 1; r <= L; r++) {
        for (size_t h = 0; h <= S; h++) {
            seq_t v = W[(r - 1) * Wd + h + 1];
            if (h > 0) v += W[(r - 1) * Wd + h - 1];
            W[r * Wd + h] = v;
        }
    }
    seq_t pos = (seq_t)0;
    for (size_t t = 0; t < S; t++)
        pos += W[(2 * t) * Wd + 0];
    size_t h = 0;
    for (size_t i = 0; i < L; i++) {
        size_t rr = L - i - 1;
        if (wd_buf[i] == '1') {
            if (h > 0)
                pos += W[rr * Wd + (h - 1)];
            h++;
        } else {
            h--;
        }
    }
    mem_free(W);
    return pos;
}
