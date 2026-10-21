/*
 * A099001  a(n) = (Sum 1/k) (Product k), where both the sum and product are
 *          over those k where 1 <= k <= n/2 and gcd(k,n) = 1.
 * https://oeis.org/A099001
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (sum_{1 <= k <= n/2, gcd(k,n)=1} 1/k) * (product_{1 <= k
 *            <= n/2, gcd(k,n)=1} k) The product times the sum of reciprocals
 *            is always an integer. We compute the product first and then sum
 *            product/k for each eligible k
 * Range:     u8 n <= 10, u16 n <= 16, u32 n <= 28, u64 n <= 42, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Euclidean algorithm for unsigned integers */
static inline idx_t gcd_u64(idx_t a, idx_t b) {
    while (b != 0) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n) {
    if (n < 2) {
        /* behavior for n < 2 is irrelevant for the offset 2 */
        return 0;
    }

    /* Compute the product of all k <= n/2 coprime to n. */
    seq_t prod = 1;
    idx_t half = n >> 1;  /* n/2 */
    for (idx_t k = 1; k <= half; ++k) {
        if (gcd_u64(k, n) == 1) {
            prod *= (seq_t)k;
        }
    }

    /* Sum prod/k over the same k's.  Since k divides prod, the division is exact. */
    seq_t sum = 0;
    for (idx_t k = 1; k <= half; ++k) {
        if (gcd_u64(k, n) == 1) {
            sum += prod / (seq_t)k;
        }
    }
    return sum;
}
