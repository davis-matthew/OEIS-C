/*
 * A006212  Number of down-up permutations of n+3 starting with n+1.
 * https://oeis.org/A006212
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = E(n+2) - E(n), where E(j) = A000111(j) are the Euler
 *            (up/down) numbers, computed with the Seidel-Entringer
 *            boustrophedon triangle (in-place, alternating direction)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 13, u64 n <= 22, u128 n <= 36,
 *            big any n
 * Validated: standard, pass, 39 terms through n = 470 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/mem.h"

static seq_t *grow_zero(seq_t *old, idx_t oldlen, idx_t newlen) {
    seq_t *p = mem_new(seq_t, (size_t)newlen);
    if (p == NULL) return NULL;
    mem_set(p, 0, (size_t)newlen * sizeof(seq_t));
    if (old != NULL) {
        mem_copy(p, old, (size_t)oldlen * sizeof(seq_t));
        mem_free(old);
    }
    return p;
}

static seq_t *eul = 0;     /* eul[j] = A000111(j) for j < eul_cnt */
static idx_t eul_cnt = 0;
static seq_t *work = 0;    /* boustrophedon working row */
static idx_t work_len = 0;

static void compute_euler(idx_t N) {
    idx_t i, j, t;
    idx_t len = 2 * N + 8;
    long long off = (long long)N + 4;
    long long k = 0, e = 1;
    seq_t Am;

    eul = grow_zero(eul, eul_cnt, N);
    if (len > work_len) {
        work = grow_zero(work, work_len, len);
        work_len = len;
    }
    for (t = 0; t < work_len; t++) work[t] = 0;

    work[off + 0] = 1;
    for (i = 0; i < N; i++) {
        Am = 0;
        work[off + k + e] = 0;
        e = -e;
        for (j = 0; j <= i; j++) {
            Am += work[off + k];
            work[off + k] = Am;
            k += e;
        }
        eul[i] = Am;
    }
    eul_cnt = N;
}

seq_t a(idx_t n) {
    idx_t need = n + 3;   /* need E(0..n+2) */
    if (need > eul_cnt) {
        idx_t N = 2 * eul_cnt;
        if (N < need) N = need;
        if (N < 32) N = 32;
        compute_euler(N);
    }
    return eul[n + 2] - eul[n];
}
