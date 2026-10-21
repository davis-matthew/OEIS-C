/*
 * A204930  Ordered differences of factorials.
 * https://oeis.org/A204930
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = k! – j! where k = ceil((1 + sqrt(1 + 8n)) / 2) (the
 *            smallest k with k(k-1)/2 ≥ n) j = n – (k-1)(k-2)/2 (the offset
 *            within the k‑th block)
 * Range:     u8 n <= 10, u16 n <= 28, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factorial.h"

/* integer square‑root (floor) for 64‑bit unsigned values. */
static idx_t isqrt_u64(idx_t v)
{
    idx_t low = 0, high = v;
    while (low < high) {
        idx_t mid = low + ((high - low + 1) >> 1);
        if (mid <= v / mid)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    /* n ≥ 1. For n == 0 (outside the offset) return 0 as a safe default. */
    if (n == 0) return (seq_t)0;

    /* Find k such that k(k-1)/2 ≥ n. */
    idx_t v = 1 + 8 * n;                 /* 1 + 8n fits in 64‑bit here. */
    idx_t root = isqrt_u64(v);           /* floor(sqrt(1+8n)). */
    idx_t k = (1 + root) / 2;            /* ceil((1+sqrt) / 2) may be too low. */
    if (k * (k - 1) / 2 < n) k++;        /* adjust if the ceil was needed. */

    /* Determine the j value within this block. */
    idx_t j = n - (k - 1) * (k - 2) / 2;

    /* Compute the factorial difference. */
    seq_t fk = factorial(k);
    seq_t fj = factorial(j);
    return fk - fj;
}
