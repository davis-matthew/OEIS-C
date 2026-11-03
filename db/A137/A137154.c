/*
 * A137154  a(n) = Sum_{k=0..n} binomial(2^k + n-k-1, n-k); equals the row
 *          sums of triangle A137153.
 * https://oeis.org/A137154
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(2^k + n-k-1, n-k)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 17, u128 n <= 19,
 *            big any n
 * Validated: partial, pass, 20 terms through n = 19 (2026-11-01)
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/pow2.h"

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

/* Compute binomial(N, k) for a small k (k <= 10000) using multiplicative
   formula. N is a seq_t value that may be arbitrarily large. */
static seq_t binom_seq(seq_t N, idx_t k)
{
    if (k == 0)
        return 1;
    seq_t result = 1;
    for (idx_t i = 1; i <= k; ++i) {
        /* result = result * (N - (i-1)) / i  */
        seq_t factor = N - (seq_t)(i - 1);
        result = (result * factor) / (seq_t)i;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t pow2k = 1;                     /* 2^0 initially */
    for (idx_t k = 0; k <= n; ++k) {
        if (k == n) {                    /* binomial(...,0) = 1 */
            sum += 1;
        } else {
            idx_t lower = n - k;          /* n-k */
            seq_t lower_seq = (seq_t)lower;
            seq_t sub = (seq_t)(n - k - 1);        /* n-k-1 (>=0 here) */
            seq_t N = pow2k + sub;                    /* 2^k + n-k-1 */
            seq_t term = binom_seq(N, lower);
            sum += term;
        }
        /* Prepare next power of two: 2^(k+1) = 2^k * 2 */
        pow2k = mul_small(pow2k, 2);
    }
    return sum;
}
