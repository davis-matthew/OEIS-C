/*
 * A083253  Smallest number k for which abs(A045763(k) - A073757(k)) = n,
 *          i.e., signed difference of size of related and unrelated sets to k
 *          equals either n or -n.
 * https://oeis.org/A083253
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = min{k : |A045763(k) - A073757(k)| = n}, where the
 *            difference is k + 2 - 2*(d(k) + phi(k)). Computed by a linear
 *            sieve with a doubling limit
 * Range:     u8 n <= 26, u16 n <= 257, u32 n <= 257, u64 n <= 257,
 *            u128 n <= 257, big any n
 * Validated: standard, pass, 80 terms through n = 253 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint32_t *A083253_first = NULL;
static uint64_t A083253_lim = 0;

static void A083253_build(uint64_t N)
{
    uint32_t *phi = (uint32_t*)malloc((N + 1) * sizeof(uint32_t));
    uint32_t *dv  = (uint32_t*)malloc((N + 1) * sizeof(uint32_t));
    uint8_t  *ex  = (uint8_t*)malloc(N + 1);
    uint32_t *pr  = (uint32_t*)malloc((N / 3 + 100) * sizeof(uint32_t));
    uint32_t *fi  = (uint32_t*)calloc(N + 1, sizeof(uint32_t));
    size_t pc = 0;
    phi[1] = 1; dv[1] = 1; ex[1] = 0;
    for (uint64_t i = 2; i <= N; ++i) {
        if (phi[i - 0] == 0 && 0) {}
        /* detect prime via dv==0 (unset) */
        if (dv[i] == 0 || 0) {}
    }
    /* initialize unset markers */
    for (uint64_t i = 2; i <= N; ++i) dv[i] = 0;
    for (uint64_t i = 2; i <= N; ++i) {
        if (dv[i] == 0) {
            pr[pc++] = (uint32_t)i;
            phi[i] = (uint32_t)(i - 1);
            dv[i] = 2;
            ex[i] = 1;
        }
        for (size_t j = 0; j < pc; ++j) {
            uint64_t p = pr[j];
            uint64_t m = i * p;
            if (m > N) break;
            if (i % p == 0) {
                phi[m] = (uint32_t)(phi[i] * p);
                ex[m] = (uint8_t)(ex[i] + 1);
                dv[m] = dv[i] / (uint32_t)(ex[i] + 1) * (uint32_t)(ex[i] + 2);
                break;
            } else {
                phi[m] = phi[i] * (uint32_t)(p - 1);
                ex[m] = 1;
                dv[m] = dv[i] * 2;
            }
        }
    }
    for (uint64_t k = 1; k <= N; ++k) {
        int64_t f = (int64_t)k + 2 - 2 * ((int64_t)dv[k] + (int64_t)phi[k]);
        if (f < 0) f = -f;
        if ((uint64_t)f <= N && fi[f] == 0) fi[f] = (uint32_t)k;
    }
    free(phi); free(dv); free(ex); free(pr);
    free(A083253_first);
    A083253_first = fi;
    A083253_lim = N;
}

seq_t a(idx_t n)
{
    const uint64_t CAP = (uint64_t)1 << 25;
    uint64_t nn = (uint64_t)n;
    if (A083253_first && nn <= A083253_lim && A083253_first[nn] != 0)
        return (seq_t)A083253_first[nn];
    uint64_t N = A083253_lim ? A083253_lim * 2 : 4096;
    if (N < nn + 3) N = nn + 3;
    for (;;) {
        if (N > CAP && A083253_lim >= CAP) return (seq_t)0;
        if (N > CAP) N = CAP;
        A083253_build(N);
        if (nn <= N && A083253_first[nn] != 0)
            return (seq_t)A083253_first[nn];
        if (N >= CAP) return (seq_t)0;
        N *= 2;
    }
}
