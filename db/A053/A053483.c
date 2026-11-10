/*
 * A053483  Euler transform of A029767.
 * https://oeis.org/A053483
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = E(n+1), where E is the Euler transform of b(d) =
 *            A029767(d) = (d-1)!*(2^d-1): Sum_{k>=0} E(k) x^k =
 *            Product_{d>=1} (1 - x^d)^(-b(d)), and (1 - x^d)^(-b) =
 *            Sum_{j>=0} binomial(b+j-1, j) x^(d*j). Computed as the product,
 *            so no intermediate value is larger than the result
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 28,
 *            big any n
 * Validated: standard, pass, 35 terms through n = 379 (2026-11-07)
 * Helpers:   gcd, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"
#include "../utils/gcd.h"

static seq_t *A053483_E = NULL;
static idx_t A053483_cnt = 0;

static void A053483_build(idx_t N)
{
    seq_t *E = mem_new(seq_t, N + 1);
    seq_t *coef = mem_new(seq_t, N + 1);
    if (!E || !coef) exit(EXIT_FAILURE);

    E[0] = (seq_t)1;
    seq_t fact = (seq_t)1, p2 = (seq_t)1;
    for (idx_t d = 1; d <= N; ++d) {
        if (d > 1) fact *= (seq_t)(d - 1);
        p2 *= (seq_t)2;
        seq_t b = fact * (p2 - (seq_t)1);   /* A029767(d) */

        idx_t jmax = N / d;
        coef[0] = (seq_t)1;
        for (idx_t j = 1; j <= jmax; ++j) {
            /* coef[j] = coef[j-1] * (b+j-1) / j, done exactly without overflow */
            seq_t c = coef[j - 1];
            seq_t t = b + (seq_t)(j - 1);
            seq_t jj = (seq_t)j;
            seq_t g = gcd(c, jj);
            if (g == (seq_t)0) g = (seq_t)1;
            seq_t q = jj / g;
            coef[j] = (c / g) * (t / q);
        }

        /* multiply E by Sum_j coef[j] x^(d*j), in place, descending k */
        for (idx_t k = N; k >= d; --k) {
            seq_t s = E[k];
            for (idx_t j = 1; j * d <= k; ++j)
                s += coef[j] * E[k - j * d];
            E[k] = s;
        }
    }

    mem_free(coef);
    if (A053483_E) mem_free(A053483_E);
    A053483_E = E;
    A053483_cnt = N + 1;
}

seq_t a(idx_t n)
{
    idx_t N = n + 1;
    if (N >= A053483_cnt) {
        idx_t target = N;
        if (target < 32) target = 32;
        A053483_build(target);
    }
    return A053483_E[N];
}
