/*
 * A004778  Where records occur in A038548.
 * https://oeis.org/A004778
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   no closed form. a(n) = n-th highly composite number h_i
 *            (A002182) with ceil(d(h_i)/2) > ceil(d(h_{i-1})/2), i.e. records
 *            of A038548. Highly composite numbers <= X are found by a
 *            Pareto-frontier sweep over the primes: keep (value, number of
 *            divisors) records, extending by p^e
 * Range:     u8 n <= 9, u16 n <= 25, u32 n <= 70, u64 n <= 167, u128 n <= 401,
 *            big any n
 * Helpers:   alloc_zero, grow_zero, is_prime, release, seq_is_fixed_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/alloc_zero.h"
#include "../utils/grow_zero.h"
#include "../utils/release.h"
#include "../utils/is_prime.h"
#include "../utils/seq_is_fixed_width.h"

static seq_t *T = 0;
static idx_t Tn = 0, Tlen = 0;
static bool fixed_done = false;
static idx_t bits = 64;

static void build(seq_t X) {
    uint64_t primes[4096];
    int K = 0;
    seq_t prim = 1;
    uint64_t q = 2;
    while (K < 4096) {
        if (is_prime(q)) {
            if (prim > X / (seq_t)q) break;
            prim *= (seq_t)q;
            primes[K++] = q;
        }
        q++;
    }

    idx_t F = 1, Vlen = 1;
    seq_t *V = alloc_zero(1), *D = alloc_zero(1);
    V[0] = 1;
    D[0] = 1;

    for (int k = 0; k < K; k++) {
        seq_t p = (seq_t)primes[k];
        idx_t E = 1;
        seq_t t = 1;
        while (t <= X / p) { t *= p; E++; }
        seq_t *PW = alloc_zero(E);
        seq_t *cur = alloc_zero(E);
        idx_t *pos = (idx_t *)malloc(E * sizeof(idx_t));
        idx_t *heap = (idx_t *)malloc(E * sizeof(idx_t));
        PW[0] = 1;
        for (idx_t e = 1; e < E; e++) PW[e] = PW[e - 1] * p;
        idx_t hs = 0;
        for (idx_t e = 0; e < E; e++) {
            pos[e] = 0;
            cur[e] = PW[e];
            heap[hs++] = e;          /* increasing values: already a heap */
        }

        idx_t cap = F + E + 16, NF = 0;
        seq_t *NV = alloc_zero(cap), *ND = alloc_zero(cap);
        seq_t best = 0;

        while (hs > 0) {
            idx_t e = heap[0];
            idx_t i = pos[e];
            seq_t dv = D[i] * (seq_t)(e + 1);
            if (dv > best) {
                if (NF == cap) {
                    idx_t nc = cap * 2;
                    NV = grow_zero(NV, cap, nc);
                    ND = grow_zero(ND, cap, nc);
                    cap = nc;
                }
                NV[NF] = cur[e];
                ND[NF] = dv;
                NF++;
                best = dv;
            }
            pos[e]++;
            bool alive = pos[e] < F;
            if (alive && V[pos[e]] > X / PW[e]) alive = false;
            if (alive && D[F - 1] * (seq_t)(e + 1) <= best) alive = false;
            if (alive) {
                cur[e] = V[pos[e]] * PW[e];
            } else {
                heap[0] = heap[--hs];
            }
            /* sift down */
            idx_t m = 0;
            for (;;) {
                idx_t l = 2 * m + 1, r = l + 1, s = m;
                if (l < hs && cur[heap[l]] < cur[heap[s]]) s = l;
                if (r < hs && cur[heap[r]] < cur[heap[s]]) s = r;
                if (s == m) break;
                idx_t tmp = heap[m]; heap[m] = heap[s]; heap[s] = tmp;
                m = s;
            }
        }

        release(PW, E);
        release(cur, E);
        free(pos);
        free(heap);
        release(V, Vlen);
        release(D, Vlen);
        V = NV;
        D = ND;
        F = NF;
        Vlen = cap;
    }

    if (T) release(T, Tlen);
    T = alloc_zero(F);
    Tlen = F;
    Tn = 0;
    seq_t bestc = 0;
    for (idx_t i = 0; i < F; i++) {
        seq_t c = (D[i] + 1) / 2;
        if (c > bestc) {
            bestc = c;
            T[Tn++] = V[i];
        }
    }
    release(V, Vlen);
    release(D, Vlen);
}

seq_t a(idx_t n) {
    while (Tn < n) {
        if (seq_is_fixed_width()) {
            if (fixed_done) return 0;
            build((seq_t)~(seq_t)0);
            fixed_done = true;
        } else {
            build((seq_t)1 << bits);
            bits = bits * 3 / 2;
        }
    }
    return T[n - 1];
}
