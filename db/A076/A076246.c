/*
 * A076246  Totients of those numbers at which values of A051547 increase: in
 *          these consecutive terms new prime powers arise, i.e., which did
 *          not occur in neither of preceding terms.
 * https://oeis.org/A076246
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = phi(A076245(n+1)): the totients of those k >= 3 where
 *            A051547(k) > A051547(k-1), i.e. phi(k) brings a new prime power
 *            not seen in phi(1..k-1); found by a sieve scan
 * Range:     u8 n <= 29, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Validated: partial, pass, 56 terms through n = 56 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static uint64_t *A076246_res = NULL;
static idx_t A076246_cnt = 0, A076246_cap = 0;

static void A076246_build(idx_t limit)
{
    uint32_t *spf = (uint32_t *)malloc((limit + 1) * sizeof(uint32_t));
    uint32_t *phi = (uint32_t *)malloc((limit + 1) * sizeof(uint32_t));
    uint8_t *maxe = (uint8_t *)calloc(limit + 1, 1);
    idx_t i, j, k;
    if (!spf || !phi || !maxe) { free(spf); free(phi); free(maxe); return; }
    for (i = 0; i <= limit; i++) { spf[i] = 0; phi[i] = (uint32_t)i; }
    for (i = 2; i <= limit; i++) {
        if (spf[i] == 0) {
            for (j = i; j <= limit; j += i) {
                if (spf[j] == 0) spf[j] = (uint32_t)i;
                phi[j] -= phi[j] / (uint32_t)i;
            }
        }
    }
    A076246_cnt = 0;
    for (k = 2; k <= limit; k++) {
        uint32_t x = phi[k];
        int inc = 0;
        while (x > 1) {
            uint32_t p = spf[x];
            uint8_t e = 0;
            while (x % p == 0) { x /= p; e++; }
            if (e > maxe[p]) { maxe[p] = e; inc = 1; }
        }
        if (inc) {
            if (A076246_cnt >= A076246_cap) {
                idx_t nc = A076246_cap ? A076246_cap * 2 : 256;
                uint64_t *nr = (uint64_t *)realloc(A076246_res, nc * sizeof(uint64_t));
                if (!nr) break;
                A076246_res = nr;
                A076246_cap = nc;
            }
            A076246_res[A076246_cnt++] = phi[k];
        }
    }
    free(spf); free(phi); free(maxe);
}

seq_t a(idx_t n)
{
    static idx_t limit = 0;
    if (n < 1) return 0;
    while (A076246_cnt < n) {
        idx_t old = A076246_cnt;
        limit = limit ? limit * 2 : 4096;
        A076246_build(limit);
        if (A076246_cnt <= old && A076246_cnt < n && limit > ((idx_t)1 << 31)) return 0;
    }
    return (seq_t)A076246_res[n - 1];
}
