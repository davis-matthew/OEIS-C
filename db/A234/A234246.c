/*
 * A234246  a(n) = |{0 < k < n: k*phi(n-k) + 1 is a square}|, where phi(.) is
 *          Euler's totient function.
 * https://oeis.org/A234246
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{0 < k < n : k*phi(n-k)+1 is a square}|
 * Range:     u8 n <= 100, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   calc_phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/calc_phi.h"

/* Integer square root of an unsigned 64‑bit value.
   Uses only integer arithmetic and avoids overflow. */
static inline idx_t isqrt_u64(uint64_t n) {
    if (n < 2) return n;
    uint64_t left = 1, right = n / 2 + 1;
    while (left <= right) {
        uint64_t mid = left + (right - left) / 2;
        if (mid <= n / mid)      /* mid*mid <= n, no overflow */
            left = mid + 1;
        else
            right = mid - 1;
    }
    return right; /* floor(sqrt(n)) */
}

/* Count the k satisfying the condition. */
seq_t a(idx_t n)
{
    if (n < 1)          /* undefined behaviour for n==0, return 0 */
        return 0;

    seq_t count = 0;

    for (idx_t k = 1; k < n; ++k) {
        idx_t m = n - k;                 /* m = n-k, 1 <= m < n */
        seq_t phi_m = calc_phi((seq_t)m); /* Euler's totient of m   */

        /* Compute k * phi_m + 1 safely in 64 bits */
        uint64_t val = (uint64_t)k * (uint64_t)phi_m + 1ULL;

        uint64_t root = isqrt_u64(val);
        if (root * root == val)
            ++count;
    }

    return count;
}
