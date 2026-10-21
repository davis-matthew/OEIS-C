/*
 * A217486  Binomial convolution of the numbers in sequence A080253.
 * https://oeis.org/A217486
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^n * t(n+1) where t is the ordered Bell numbers. t(0)=1
 *            and for n≥1, t(n)=∑_{k=1}^{n} binom(n,k)*t(n–k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 14, u128 n <= 25,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/pow2.h"

#define MAX_PRECOMP 20000   /* enough for n up to about 10000 */

static seq_t t[MAX_PRECOMP + 2];
static idx_t precomputed_max = 0;

/* Ensure that t[0 … n+1] have been computed. */
static void ensure_precomputed(idx_t n)
{
    if (n + 1 <= precomputed_max) return;

    /* Initialise t[0] on first use. */
    if (precomputed_max == 0) t[0] = 1;

    for (idx_t i = precomputed_max + 1; i <= n + 1; ++i) {
        t[i] = 0;
        seq_t bin = 1;
        for (idx_t k = 1; k <= i; ++k) {
            bin = bin * (seq_t)(i - k + 1);   /* binom(i,k) multiplicatively */
            bin = bin / (seq_t)k;
            t[i] += bin * t[i - k];
        }
    }
    precomputed_max = n + 1;
}

seq_t a(idx_t n)
{
    if (n > MAX_PRECOMP) {
        /* The problem guarantees n will not exceed about 10000. */
    }
    ensure_precomputed(n);
    seq_t res = pow2(n);
    res *= t[n + 1];
    return res;
}
