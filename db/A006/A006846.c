/*
 * A006846  Hammersley's polynomial p_n(1).
 * https://oeis.org/A006846
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   G.f. is the S-fraction 1/(1-x/(1-x/(1-3x/(1-4x/(1-7x/...)))))
 *            with coefficients ceiling(h^2/4). Contracted to a J-fraction
 *            with b_k = 2k^2+k+1 and lambda_k = k^2*(k^2-k+1), this gives the
 *            Motzkin-path recurrence T(0,0) = 1, T(m,k) = T(m-1,k-1) +
 *            b_k*T(m-1,k) + lambda_{k+1}*T(m-1,k+1), a(n) = T(n,0)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 15, u128 n <= 24,
 *            big any n
 * Helpers:   alloc_zero, grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/alloc_zero.h"
#include "../utils/grow_zero.h"

static seq_t *row = NULL;   /* row[k] = T(m,k), k = 0..m, zero beyond */
static idx_t rowcap = 0;
static seq_t *av = NULL;    /* av[i] = a(i) for i < avlen */
static idx_t avcap = 0;
static idx_t avlen = 0;
static idx_t curm = 0;

seq_t a(idx_t n)
{
    if (row == NULL) {
        idx_t cap = 16;
        row = alloc_zero(cap);
        rowcap = cap;
        av = alloc_zero(cap);
        avcap = cap;
        row[0] = 1;
        av[0] = 1;
        avlen = 1;
        curm = 0;
    }
    if (n < avlen) return av[n];

    if (n + 3 > rowcap) {
        idx_t nc = 2 * (n + 3);
        row = grow_zero(row, rowcap, nc);
        rowcap = nc;
    }
    if (n + 1 > avcap) {
        idx_t nc = 2 * (n + 1);
        av = grow_zero(av, avcap, nc);
        avcap = nc;
    }

    while (avlen <= n) {
        seq_t prev = 0;
        idx_t k;
        for (k = 0; k <= curm + 1; k++) {
            seq_t K = (seq_t)k;
            seq_t K1 = (seq_t)(k + 1);
            seq_t bk = (seq_t)2 * K * K + K + 1;
            seq_t lam = K1 * K1 * (K1 * K1 - K1 + 1);
            seq_t cur = row[k];
            seq_t nv = prev + bk * cur + lam * row[k + 1];
            row[k] = nv;
            prev = cur;
        }
        curm++;
        av[avlen] = row[0];
        avlen++;
    }
    return av[n];
}
