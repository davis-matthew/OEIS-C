/*
 * A000081  Number of unlabeled rooted trees with n nodes (or connected
 *          functions with a fixed point).
 * https://oeis.org/A000081
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, core, nice, eigen
 * Formula:   a(n) = [x^(n-1)] Product_{k>=1} (1-x^k)^(-a(k)), a(0) = 0, a(1)
 *            = 1 (Euler transform of the sequence is the sequence shifted by
 *            one)
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 25, u64 n <= 47, u128 n <= 88,
 *            big any n
 * Helpers:   a81_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/a81_gcd.h"

#define A81_MAX 32768

static seq_t A81_T[A81_MAX + 2];
static seq_t A81_P[A81_MAX + 2];
static seq_t A81_M[A81_MAX + 2];
static idx_t A81_cap = 0;

/* computes a(0..N) into A81_T */
static void a81_build(idx_t N)
{
    idx_t L = N;               /* P holds coefficients 0..L-1 */
    seq_t *P = A81_P;
    seq_t *M = A81_M;
    seq_t *T = A81_T;
    idx_t i, k, m, j;

    for (i = 0; i <= L; i++) { P[i] = 0; M[i] = 0; }
    for (i = 0; i <= N + 1; i++) T[i] = 0;
    P[0] = 1;
    T[0] = 0;
    T[1] = 1;

    for (k = 1; k < N; k++) {
        seq_t c = T[k];
        idx_t J = (L - 1) / k;
        /* M[j] = binomial(c+j-1, j), built without oversized intermediates */
        M[0] = 1;
        for (j = 1; j <= J; j++) {
            unsigned long long r = (unsigned long long)(M[j - 1] % (seq_t)j);
            unsigned long long g = a81_gcd(r, (unsigned long long)j);
            seq_t x = M[j - 1] / (seq_t)g;
            seq_t y = (c + (seq_t)(j - 1)) / (seq_t)((unsigned long long)j / g);
            M[j] = x * y;
        }
        /* P *= (1-x^k)^(-c), in place, high coefficients first */
        for (m = L - 1; m >= k; m--) {
            seq_t sum = P[m];
            idx_t r = m - k;
            for (j = 1;; j++) {
                sum += M[j] * P[r];
                if (r < k) break;
                r -= k;
            }
            P[m] = sum;
        }
        T[k + 1] = P[k];
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n > A81_MAX) n = A81_MAX;
    if (n > A81_cap) {
        idx_t nc = A81_cap + A81_cap / 2;
        if (nc < n) nc = n;
        if (nc < 16) nc = 16;
        if (nc > A81_MAX) nc = A81_MAX;
        a81_build(nc);
        A81_cap = nc;
    }
    return A81_T[n];
}
