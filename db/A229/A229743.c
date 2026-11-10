/*
 * A229743  Positions of records in A096303.
 * https://oeis.org/A229743
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   no closed form; a search over n. Let f(n) = A096303(n), the *
 *            number of steps of n -> n + A000120(n) needed to reach the
 *            trajectory * of 1 (A010062). Then f(n) = 0 if n is in A010062,
 *            else * f(n) = 1 + f(n + A000120(n)). a(n) is the n-th position
 *            where f sets * a new strict record. * * f is compu...
 * Range:     u8 n <= 6, u16 n <= 20, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Validated: partial, pass, 34 terms through n = 34 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static unsigned A229743_pop(uint64_t x)
{
#if defined(__GNUC__) || defined(__clang__)
    return (unsigned)__builtin_popcountll((unsigned long long)x);
#else
    x = x - ((x >> 1) & 0x5555555555555555ULL);
    x = (x & 0x3333333333333333ULL) + ((x >> 2) & 0x3333333333333333ULL);
    x = (x + (x >> 4)) & 0x0F0F0F0F0F0F0F0FULL;
    return (unsigned)((x * 0x0101010101010101ULL) >> 56);
#endif
}

#define A229743_BS ((uint64_t)1 << 20)

static uint64_t *A229743_rec = NULL;   /* record positions found so far */
static size_t A229743_cnt = 0, A229743_cap = 0;
static uint64_t A229743_done = 0;      /* 1..done fully processed */
static uint64_t A229743_tnext = 1;     /* smallest trajectory element > done */
static int64_t A229743_best = -1;      /* max f over 1..done */

static uint64_t *A229743_tp = NULL;    /* trajectory points within a block */
static uint64_t *A229743_stn = NULL;   /* stack: positions */
static uint32_t *A229743_stv = NULL;   /* stack: values */
static size_t A229743_stcap = 0;

static void A229743_push_rec(uint64_t n)
{
    if (A229743_cnt == A229743_cap) {
        A229743_cap = A229743_cap ? 2 * A229743_cap : 256;
        A229743_rec = (uint64_t *)realloc(A229743_rec, A229743_cap * sizeof(uint64_t));
        if (!A229743_rec) abort();
    }
    A229743_rec[A229743_cnt++] = n;
}

/* Process the segment [L, L + K*BS - 1]. */
static void A229743_segment(uint64_t L, uint64_t K)
{
    uint64_t R = L - 1 + K * A229743_BS;
    uint64_t *ck = (uint64_t *)malloc((size_t)K * sizeof(uint64_t));
    if (!ck) abort();
    if (!A229743_tp) {
        A229743_tp = (uint64_t *)malloc((size_t)(A229743_BS + 2) * sizeof(uint64_t));
        if (!A229743_tp) abort();
    }

    /* checkpoints of the trajectory of 1 at each block start */
    uint64_t t = A229743_tnext;
    for (uint64_t k = 0; k < K; k++) {
        uint64_t lo = L + k * A229743_BS;
        while (t < lo) t += A229743_pop(t);
        ck[k] = t;
    }
    while (t <= R) t += A229743_pop(t);
    uint64_t tafter = t;   /* first trajectory element > R */

    /* f values for R+1 .. R+64 by direct simulation */
    uint32_t ring[128];
    for (int d = 1; d <= 64; d++) {
        uint64_t c = R + (uint64_t)d, tr = tafter;
        uint32_t steps = 0;
        for (;;) {
            while (tr < c) tr += A229743_pop(tr);
            if (tr == c) break;
            c += A229743_pop(c);
            steps++;
        }
        ring[(R + (uint64_t)d) & 127] = steps;
    }

    size_t sp = 0;
    for (uint64_t kk = K; kk-- > 0; ) {
        uint64_t lo = L + kk * A229743_BS, hi = lo + A229743_BS - 1;
        size_t tc = 0;
        uint64_t u = ck[kk];
        while (u <= hi) { A229743_tp[tc++] = u; u += A229743_pop(u); }
        size_t j = tc;   /* tp[j-1] is the next trajectory point going down */
        for (uint64_t n = hi + 1; n-- > lo; ) {
            uint32_t f;
            if (j > 0 && A229743_tp[j - 1] == n) { f = 0; j--; }
            else f = 1 + ring[(n + A229743_pop(n)) & 127];
            ring[n & 127] = f;
            while (sp > 0 && A229743_stv[sp - 1] <= f) sp--;
            if (sp == A229743_stcap) {
                A229743_stcap = A229743_stcap ? 2 * A229743_stcap : 1024;
                A229743_stn = (uint64_t *)realloc(A229743_stn, A229743_stcap * sizeof(uint64_t));
                A229743_stv = (uint32_t *)realloc(A229743_stv, A229743_stcap * sizeof(uint32_t));
                if (!A229743_stn || !A229743_stv) abort();
            }
            A229743_stn[sp] = n;
            A229743_stv[sp] = f;
            sp++;
        }
    }

    /* stack from top (sp-1) to bottom (0): increasing n, increasing value */
    for (size_t i = sp; i-- > 0; ) {
        if ((int64_t)A229743_stv[i] > A229743_best) {
            A229743_best = (int64_t)A229743_stv[i];
            A229743_push_rec(A229743_stn[i]);
        }
    }

    A229743_tnext = tafter;
    A229743_done = R;
    free(ck);
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    while (A229743_cnt < (size_t)n) {
        uint64_t L = A229743_done + 1;
        uint64_t K = L / (8 * A229743_BS);
        if (K < 1) K = 1;
        A229743_segment(L, K);
    }
    return (seq_t)A229743_rec[n - 1];
}
