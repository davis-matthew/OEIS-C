/*
 * A102688  a(n) = (1/n)*Sum_{k=1..n} k*2^gcd(n,k).
 * https://oeis.org/A102688
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (1/n) * Σ_{k=1..n} k * 2^gcd(n,k)
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 27, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include <stdint.h>    /* for uint64_t etc., if needed */

/* Euclidean algorithm for idx_t – works for any unsigned integer type */
static inline idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) /* sequence starts at 1, 0 is a dummy value */
        return 0;

    seq_t sum = 0;           /* Σ_{k=1..n} k * 2^gcd(n,k) */
    idx_t i;

    for (i = 1; i <= n; ++i) {
        idx_t g = gcd_idx(n, i);     /* gcd(n,i) */
        seq_t pow = pow2(g);         /* 2^g as seq_t */
        sum += (seq_t)i * pow;       /* i * 2^g */
    }

    /* a(n) = sum / n, exact division as per definition */
    return sum / (seq_t)n;
}
