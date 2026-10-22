/*
 * A000713  EULER transform of 3, 2, 2, 2, 2, 2, 2, 2, ...
 * https://oeis.org/A000713
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   G.f.: 1/((1-x) * Product_{k>=1} (1-x^k)^2), i.e. the Euler
 *            transform of 3,2,2,2,...; arbitrary precision: a(0) = 1, a(n) =
 *            (1/n) * Sum_{k=1..n} (2*sigma_1(k)+1) * a(n-k); fixed width: the
 *            product is expanded by repeated partial sums (additions only)
 * Range:     u8 n <= 7, u16 n <= 19, u32 n <= 59, u64 n <= 198, u128 n <= 706,
 *            big any n
 * Helpers:   a713_is_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a713_is_big.h"

#define A713_CAP 32768

static seq_t a713_A[A713_CAP];
static idx_t a713_have = 0;
static uint64_t a713_sig[A713_CAP];
static int a713_sig_done = 0;
static int a713_mode = -1; /* 1 = arbitrary precision, 0 = fixed width */

static void a713_sieve(void) {
    idx_t i, j;
    for (i = 0; i < A713_CAP; i++) a713_sig[i] = 0;
    for (i = 1; i < A713_CAP; i++)
        for (j = i; j < A713_CAP; j += i)
            a713_sig[j] += (uint64_t)i;
    a713_sig_done = 1;
}

seq_t a(idx_t n) {
    if (n >= A713_CAP) n = A713_CAP - 1;
    if (a713_mode < 0) a713_mode = a713_is_big();

    if (n < a713_have) return a713_A[n];

    if (a713_mode) {
        idx_t i, k;
        if (!a713_sig_done) a713_sieve();
        if (a713_have == 0) {
            a713_A[0] = 1;
            a713_have = 1;
        }
        for (i = a713_have; i <= n; i++) {
            seq_t s = 0;
            for (k = 1; k <= i; k++) {
                seq_t c = (seq_t)(2 * a713_sig[k] + 1);
                s += c * a713_A[i - k];
            }
            a713_A[i] = s / (seq_t)i;
        }
        a713_have = n + 1;
    } else {
        idx_t N = n + 1, i, j, k;
        int pass;
        if (N < 2 * a713_have) N = 2 * a713_have;
        if (N > A713_CAP) N = A713_CAP;
        for (i = 0; i < N; i++) a713_A[i] = 0;
        a713_A[0] = 1;
        for (k = 1; k < N; k++)
            for (pass = 0; pass < 2; pass++)
                for (j = k; j < N; j++)
                    a713_A[j] += a713_A[j - k];
        for (j = 1; j < N; j++)
            a713_A[j] += a713_A[j - 1];
        a713_have = N;
    }
    return a713_A[n];
}
