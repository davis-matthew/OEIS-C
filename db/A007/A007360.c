/*
 * A007360  Number of partitions of n into distinct and pairwise relatively
 *          prime parts.
 * https://oeis.org/A007360
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = b(n) + b(n-1), where b(m) counts sets of pairwise coprime
 *            parts > 1 summing to m; b is computed by a DP over the primes in
 *            decreasing order, assigning each part to its largest prime
 *            factor and tracking which small primes (p^2 <= N) are already
 *            used, as a bitmask
 * Range:     u8 n <= 51, u16 n <= 167, u32 n <= 646, u64 n <= 750,
 *            u128 n <= 750, big any n
 * Helpers:   alloc_zero, cmp_desc_u64, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"
#include "../utils/cmp_desc_u64.h"

typedef struct { uint64_t mask; seq_t *row; } a7360_ent;

static a7360_ent *E;
static idx_t En, Ecap;
static long long *H;
static idx_t Hcap;
static idx_t RL;

static seq_t *Bv;
static idx_t BN;

static idx_t hslot(uint64_t m) {
    uint64_t h = m * 0x9E3779B97F4A7C15ULL;
    return (idx_t)(h >> 32) & (Hcap - 1);
}

static void h_rebuild(void) {
    idx_t i;
    free(H);
    Hcap = 16;
    while (Hcap < 2 * En + 2) Hcap <<= 1;
    H = (long long *)malloc((size_t)Hcap * sizeof(long long));
    for (i = 0; i < Hcap; i++) H[i] = -1;
    for (i = 0; i < En; i++) {
        idx_t s = hslot(E[i].mask);
        while (H[s] >= 0) s = (s + 1) & (Hcap - 1);
        H[s] = (long long)i;
    }
}

static long long h_find(uint64_t m) {
    idx_t s = hslot(m);
    while (H[s] >= 0) {
        if (E[H[s]].mask == m) return H[s];
        s = (s + 1) & (Hcap - 1);
    }
    return -1;
}

static seq_t *h_insert(uint64_t m, seq_t *row) {
    if (En == Ecap) {
        Ecap = Ecap ? 2 * Ecap : 16;
        E = (a7360_ent *)realloc(E, (size_t)Ecap * sizeof(a7360_ent));
    }
    E[En].mask = m;
    E[En].row = row;
    En++;
    if (2 * En + 2 > Hcap) {
        h_rebuild();
    } else {
        idx_t s = hslot(m);
        while (H[s] >= 0) s = (s + 1) & (Hcap - 1);
        H[s] = (long long)(En - 1);
    }
    return row;
}

static seq_t *get_row(uint64_t m) {
    long long i = h_find(m);
    if (i >= 0) return E[i].row;
    return h_insert(m, alloc_zero(RL));
}

static void drop_bit(uint64_t bit) {
    a7360_ent *old = E;
    idx_t on = En, i, s;
    E = NULL; En = 0; Ecap = 0;
    h_rebuild();
    for (i = 0; i < on; i++) {
        uint64_t m = old[i].mask & ~bit;
        long long j = h_find(m);
        if (j >= 0) {
            seq_t *dst = E[j].row;
            for (s = 0; s < RL; s++) dst[s] += old[i].row[s];
            release(old[i].row, RL);
        } else {
            h_insert(m, old[i].row);
        }
    }
    free(old);
}

static void build(idx_t N) {
    idx_t i, j, P, s;
    uint32_t *gpf = (uint32_t *)calloc((size_t)N + 1, sizeof(uint32_t));
    uint64_t *mb = (uint64_t *)calloc((size_t)N + 1, sizeof(uint64_t));
    int *bitof = (int *)malloc(((size_t)N + 1) * sizeof(int));
    idx_t *optk = (idx_t *)malloc(((size_t)N + 1) * sizeof(idx_t));
    uint64_t *optb = (uint64_t *)malloc(((size_t)N + 1) * sizeof(uint64_t));
    idx_t *ink = (idx_t *)malloc(64 * sizeof(idx_t));
    int nb = 0;
    seq_t *row0;
    long long f;

    for (i = 0; i <= N; i++) bitof[i] = -1;
    for (i = 2; i <= N; i++) {
        if (gpf[i] != 0) continue;
        if (i * i <= N && nb < 64) bitof[i] = nb++;
        for (j = i; j <= N; j += i) {
            gpf[j] = (uint32_t)i;
            if (bitof[i] >= 0) mb[j] |= (uint64_t)1 << bitof[i];
        }
    }

    RL = N + 1;
    E = NULL; En = 0; Ecap = 0; H = NULL;
    h_rebuild();
    row0 = get_row(0);
    row0[0] = 1;

    for (P = N; P >= 2; P--) {
        uint64_t pb, *snap;
        idx_t nopt = 0, nin = 0, pe, m, sn, t, q;
        if (gpf[P] != P) continue;
        pb = bitof[P] >= 0 ? ((uint64_t)1 << bitof[P]) : 0;

        for (pe = P; ; ) {
            for (m = 1; m <= N / pe; m++) {
                if (m == 1 || gpf[m] < P) {
                    if (mb[m] == 0) {
                        if (nin < 64) ink[nin++] = pe * m;
                    } else {
                        optk[nopt] = pe * m;
                        optb[nopt] = mb[m];
                        nopt++;
                    }
                }
            }
            if (pe > N / P) break;
            pe *= P;
        }

        sn = En;
        snap = (uint64_t *)malloc((size_t)(sn ? sn : 1) * sizeof(uint64_t));
        for (i = 0; i < sn; i++) snap[i] = E[i].mask;
        qsort(snap, (size_t)sn, sizeof(uint64_t), cmp_desc_u64);

        for (i = 0; i < sn; i++) {
            uint64_t mask = snap[i];
            seq_t *row;
            if (mask & pb) continue;
            row = E[h_find(mask)].row;
            for (q = 0; q < nopt; q++) {
                seq_t *dst;
                idx_t k = optk[q];
                if (optb[q] & mask) continue;
                dst = get_row(mask | optb[q]);
                for (s = 0; s + k <= N; s++)
                    if (row[s] != 0) dst[s + k] += row[s];
            }
            for (s = N + 1; s-- > 0; ) {
                if (row[s] == 0) continue;
                for (t = 0; t < nin; t++)
                    if (s + ink[t] <= N) row[s + ink[t]] += row[s];
            }
        }
        free(snap);

        if (pb) drop_bit(pb);
    }

    f = h_find(0);
    Bv = E[f].row;
    for (i = 0; i < En; i++)
        if ((long long)i != f) release(E[i].row, RL);
    free(E); E = NULL; En = 0; Ecap = 0;
    free(H); H = NULL; Hcap = 0;
    BN = N;

    free(gpf); free(mb); free(bitof); free(optk); free(optb); free(ink);
}

seq_t a(idx_t n) {
    seq_t r;
    if (Bv == NULL || n > BN) {
        idx_t N = n;
        if (Bv != NULL) {
            if (N < BN + BN / 2) N = BN + BN / 2;
            release(Bv, BN + 1);
            Bv = NULL;
        }
        if (N < 64) N = 64;
        build(N);
    }
    r = Bv[n];
    if (n >= 1) r += Bv[n - 1];
    return r;
}
