/*
 * A051010  Triangle T(m,n) giving of number of steps in the Euclidean
 *          algorithm for gcd(m,n) with 0<=m<n.
 * https://oeis.org/A051010
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, tabl
 * Formula:   a(n) = number of steps minus one in the Euclidean algorithm for
 *            gcd(m,n), where (m,n) are enumerated row‑wise with 0 <= m < n,
 *            offset 1
 * Range:     u8 n <= 5050, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   euclid_steps, triangular
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/euclid_steps.h"
#include "../utils/triangular.h"

seq_t a(idx_t n)
{
    idx_t low = 1, high = n, row = n;
    while (low <= high)
    {
        idx_t mid = (low + high) >> 1;
        if (triangular(mid) >= n)
        {
            row = mid;
            high = mid - 1;
        }
        else
        {
            low = mid + 1;
        }
    }
    idx_t prev = triangular(row - 1);
    idx_t m = n - prev - 1;

    return euclid_steps((seq_t)m, (seq_t)row);
}
