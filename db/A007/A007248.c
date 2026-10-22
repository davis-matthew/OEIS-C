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
 * Helpers:   grow_zero, r4_u, sig_u
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/grow_zero.h"
#include "../utils/r4_u.h"
#include "../utils/sig_u.h"

static seq_t *A = NULL;
static idx_t *P = NULL;
static idx_t alen = 0;

seq_t a(idx_t n)
{
    if (n >= alen) {
        idx_t newlen = n + 1, m, k;
        A = grow_zero(A, alen, newlen);
        P = (idx_t *)realloc(P, (size_t)newlen * sizeof(idx_t));
        for (m = alen; m < newlen; m++) P[m] = (idx_t)sig_u(2 * m + 1);
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
