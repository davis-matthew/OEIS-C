/*
 * A038267  Duplicate of A027466.
 * https://oeis.org/A038267
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   * a(n) = C(k, r) * 7^(k - r) * where k = ⌊(√(8n + 1) - 1) / 2⌋
 *            is the largest integer with k(k+1)/2 ≤ n, * r = n - k(k+1)/2 (0
 *            ≤ r ≤ k). * C(k,r) is the binomial coefficient. * (This yields
 *            a(0)=1, a(1)=7, a(2)=1, a(3)=49, …)
 * Range:     i8 n <= 5, i16 n <= 17, i32 n <= 17, i64 n <= 17, i128 n <= 17,
 *            big any n
 * Helpers:   triangular_floor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/triangular_floor.h"

/* Binomial coefficient C(n,k) using multiplicative formula.
 * Assumes seq_t supports +,-,*,/ on signed integers. */
static seq_t binom_seq(idx_t n, idx_t k)
{
    if (k > n)
        return 0;
    if (k > n - k)
        k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (seq_t)(n - k + i) / i;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* Find k such that k(k+1)/2 <= n < (k+1)(k+2)/2 */
    idx_t k = triangular_floor(n);

    /* Compute triangular number T_k = k*(k+1)/2 safely */
    __int128 tri128 = (__int128)k * (k + 1) / 2;
    idx_t t_k = (idx_t)tri128;

    idx_t r = n - t_k;            /* 0 <= r <= k */
    idx_t e = k - r;              /* exponent of 7 */

    seq_t bin = binom_seq(k, r);

    /* Compute 7^e */
    seq_t pow7 = 1;
    for (idx_t i = 0; i < e; ++i)
        pow7 *= 7;

    return bin * pow7;
}
