/*
 * A087814  Sequence gives A087655(n) for those n such that A087655(n) =
 *          A087655(n+1).
 * https://oeis.org/A087814
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   run A087655 forward: b(n) = b(|n-b(n-1)|) + b(|n-b(n-2)|) +
 *            b(|n-b(n-3)|), * with b(0)=b(1)=b(2)=b(3)=1. The initial run
 *            b(1)=b(2)=b(3)=1 gives a(0)=a(1)=1. * After that, each n with
 *            b(n)=b(n+1) adds b(n) to the sequence only if it is * larger
 *            than the last term added, so the terms keep increasing
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 18, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Validated: partial, pass, 19 terms through n = 18 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/mem.h"

static seq_t *A087814_b = NULL;
static size_t A087814_blen = 0, A087814_bcap = 0;
static seq_t *A087814_e = NULL;
static size_t A087814_elen = 0, A087814_ecap = 0;

static void A087814_pushb(seq_t v)
{
    if (A087814_blen == A087814_bcap) {
        size_t nc = A087814_bcap ? A087814_bcap * 2 : 64;
        seq_t *t = mem_resize(seq_t, A087814_b, nc);
        if (!t) abort();
        A087814_b = t;
        A087814_bcap = nc;
    }
    A087814_b[A087814_blen++] = v;
}

static void A087814_pushe(seq_t v)
{
    if (A087814_elen == A087814_ecap) {
        size_t nc = A087814_ecap ? A087814_ecap * 2 : 64;
        seq_t *t = mem_resize(seq_t, A087814_e, nc);
        if (!t) abort();
        A087814_e = t;
        A087814_ecap = nc;
    }
    A087814_e[A087814_elen++] = v;
}

static seq_t A087814_get(seq_t v, size_t m)
{
    seq_t mv = (seq_t)m;
    seq_t d = v > mv ? v - mv : mv - v;
    if (d >= mv) return 0; /* not reached for this sequence */
    return A087814_b[(size_t)(long long)d];
}

static void A087814_step(void)
{
    if (A087814_blen == 0) {
        int i;
        for (i = 0; i < 4; i++) A087814_pushb((seq_t)1);
        A087814_pushe((seq_t)1);
        A087814_pushe((seq_t)1);
        return;
    }
    size_t m = A087814_blen;
    seq_t v1 = A087814_b[m - 1], v2 = A087814_b[m - 2], v3 = A087814_b[m - 3];
    seq_t r = A087814_get(v1, m) + A087814_get(v2, m) + A087814_get(v3, m);
    A087814_pushb(r);
    if (A087814_b[m - 1] == r && r > A087814_e[A087814_elen - 1])
        A087814_pushe(r);
}

seq_t a(idx_t n)
{
    while (A087814_elen <= (size_t)n) A087814_step();
    return A087814_e[(size_t)n];
}
