/*
 * A057655  The circle problem: number of points (x,y) in square lattice with
 *          x^2 + y^2 <= n.
 * https://oeis.org/A057655
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 1 + 4*∑_{k=0}^{⌊√n⌋} ⌊√(n-k²)⌋ Computes the number of
 *            integer lattice points with x² + y² ≤ n. The implementation uses
 *            a simple isqrt routine (O(log n)) and runs in O(√n log n) time,
 *            well below the 2 s limit for n up to 10 000
 * Range:     u8 n <= 81, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square‑root: floor(sqrt(n)) for n : uint64_t.
   Uses a binary search avoiding overflow by comparing mid ≤ n/mid. */
static inline idx_t
isqrt_u64(idx_t n)
{
    idx_t low = 0, high = n, mid, best = 0;
    while (low <= high) {
        mid = (low + high) >> 1;
        if (mid == 0 || mid <= n / mid) {
            best = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return best;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t limit = isqrt_u64(n);
    for (idx_t k = 0; k <= limit; ++k) {
        idx_t t = n - k*k;
        sum += (seq_t)isqrt_u64(t);
    }
    return (seq_t)1 + (seq_t)4 * sum;
}
