/*
 * A007248  McKay-Thompson series of class 4C for the Monster group.
 * https://oeis.org/A007248
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = [x^n] E(x)/Psi(x), from 16*(theta_3/theta_2)^4 - 8 with
 *            theta_2(q)^4 = 16*q*psi(q^2)^4. Psi(x) = psi(x)^4 = Sum_{m>=0}
 *            sigma(2m+1) x^m (sums of four triangular numbers); E(x) =
 *            Sum_{m>=0} r4(2m) x^m, r4(0) = 1, r4(k) = 8*sigma(k) -
 *            32*sigma(k/4). Recurrence: a(n) = E_n - Sum_{k=1..n} sigma...
 * Range:     i8 n <= 2, i16 n <= 8, i32 n <= 32, i64 n <= 117, i128 n <= 443,
 *            big any n
 * Validated: standard, pass, 45 terms through n = 992 (2026-11-03)
 * Helpers:   mem, sigma_u64_to_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/sigma_u64_to_u64.h"
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

static uint64_t r4_u(uint64_t k)
{
    if (k == 0) return 1;
    return 8 * sigma_u64_to_u64(k) - ((k % 4 == 0) ? 32 * sigma_u64_to_u64(k / 4) : 0);
}

static seq_t *A = NULL;
static idx_t *P = NULL;
static idx_t alen = 0;

seq_t a(idx_t n)
{
    if (n >= alen) {
        idx_t newlen = n + 1, m, k;
        A = grow_zero(A, alen, newlen);
        P = (idx_t *)realloc(P, (size_t)newlen * sizeof(idx_t));
        for (m = alen; m < newlen; m++) P[m] = (idx_t)sigma_u64_to_u64(2 * m + 1);
        for (m = alen; m < newlen; m++) {
            seq_t s = 0;
            s += (idx_t)r4_u(2 * m);
            for (k = 1; k <= m; k++) s -= A[m - k] * P[k];
            A[m] = s;
        }
        alen = newlen;
    }
    return A[n];
}
