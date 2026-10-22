/*
 * A006603  Generalized Fibonacci numbers.
 * https://oeis.org/A006603
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   G.f.: (A006318(x) - x)/(1 - x + x^2 + x^3), i.e. a(n) = S(n) -
 *            [n=1] + a(n-1) - a(n-2) - a(n-3), where S = A006318 (large
 *            Schroeder numbers), S(0) = 1, S(1) = 2, (n+1)*S(n) =
 *            3*(2n-1)*S(n-1) - (n-2)*S(n-2) (equivalently S(n) = S(n-1) +
 *            Sum_{k=0..n-1} S(k)*S(n-1-k))
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 14, u64 n <= 28, u128 n <= 53,
 *            big any n
 * Helpers:   grow_zero, seq_is_fixed_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/grow_zero.h"
#include "../utils/seq_is_fixed_width.h"

static seq_t *S = NULL;
static seq_t *A = NULL;
static idx_t cap = 0;
static idx_t done = 0;

static void extend(idx_t n) {
    if (n + 1 > cap) {
        idx_t nc = cap * 2;
        if (nc < n + 1) nc = n + 1;
        if (nc < 8) nc = 8;
        S = grow_zero(S, cap, nc);
        A = grow_zero(A, cap, nc);
        cap = nc;
    }
    bool fixed = seq_is_fixed_width();
    for (idx_t i = done; i <= n; i++) {
        if (i == 0) {
            S[0] = 1;
        } else if (i == 1) {
            S[1] = 2;
        } else if (fixed) {
            /* division-free convolution: exact modulo 2^w */
            seq_t t = S[i - 1];
            for (idx_t k = 0; k < i; k++)
                t += S[k] * S[i - 1 - k];
            S[i] = t;
        } else {
            S[i] = ((seq_t)(6 * i - 3) * S[i - 1] - (seq_t)(i - 2) * S[i - 2]) / (seq_t)(i + 1);
        }
        seq_t t = S[i];
        if (i == 1) t -= 1;
        if (i >= 1) t += A[i - 1];
        if (i >= 2) t -= A[i - 2];
        if (i >= 3) t -= A[i - 3];
        A[i] = t;
    }
    done = n + 1;
}

seq_t a(idx_t n) {
    if (n >= done) extend(n);
    return A[n];
}
