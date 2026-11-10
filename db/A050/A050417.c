/*
 * A050417  Least k such that A050416(k) = n.
 * https://oeis.org/A050417
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k such that A050416(k) = n; where A050416(1) =
 *            A050416(2) = 1 and A050416(m+1) = floor(A050416(m)/3) if that is
 *            not 0 or an earlier term, else A050416(m) + A050416(m-1).
 *            Computed by generating A050416 until n appears
 * Range:     u8 n <= 5, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Validated: partial, pass, 59 terms through n = 59 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"

#define A050417_LIM (1u << 22)

static unsigned char A050417_bits[A050417_LIM];
static seq_t *A050417_big = NULL;
static size_t A050417_nbig = 0, A050417_capbig = 0;
static seq_t *A050417_t = NULL;
static size_t A050417_cnt = 0, A050417_cap = 0;

static bool A050417_seen(seq_t v)
{
    if (v < (seq_t)A050417_LIM)
        return A050417_bits[(size_t)(unsigned long long)v] != 0;
    for (size_t i = 0; i < A050417_nbig; ++i)
        if (A050417_big[i] == v) return true;
    return false;
}

static void A050417_mark(seq_t v)
{
    if (v < (seq_t)A050417_LIM) {
        A050417_bits[(size_t)(unsigned long long)v] = 1;
        return;
    }
    if (A050417_nbig >= A050417_capbig) {
        size_t nc = A050417_capbig ? A050417_capbig * 2 : 64;
        seq_t *p = mem_resize(seq_t, A050417_big, nc);
        if (!p) abort();
        A050417_big = p;
        A050417_capbig = nc;
    }
    A050417_big[A050417_nbig++] = v;
}

static void A050417_push(seq_t v)
{
    if (A050417_cnt + 1 >= A050417_cap) {
        size_t nc = A050417_cap ? A050417_cap * 2 : 1024;
        seq_t *p = mem_resize(seq_t, A050417_t, nc);
        if (!p) abort();
        A050417_t = p;
        A050417_cap = nc;
    }
    A050417_t[++A050417_cnt] = v;
    A050417_mark(v);
}

seq_t a(idx_t n)
{
    seq_t target = (seq_t)n;
    if (A050417_cnt == 0) {
        A050417_push(1);
        A050417_push(1);
    }
    while (!A050417_seen(target)) {
        seq_t prev = A050417_t[A050417_cnt];
        seq_t cand = prev / 3;
        if (cand == 0 || A050417_seen(cand))
            cand = prev + A050417_t[A050417_cnt - 1];
        A050417_push(cand);
    }
    for (size_t i = 1; i <= A050417_cnt; ++i)
        if (A050417_t[i] == target) return (seq_t)i;
    return 0;
}
