/*
 * A132919  Triangle read by rows: T(n,k) = Fibonacci(n) + k - 1.
 * https://oeis.org/A132919
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = Fibonacci(r) + k - 1, where r is the smallest integer
 *            with r(r+1)/2 >= n, and k = n - (r-1)r/2. This is the flattening
 *            of the lower‑triangular matrix with rows (Fibonacci(r), F(r)+1,
 *            …, F(r)+r-1)
 * Range:     u8 n <= 91, u16 n <= 300, u32 n <= 1128, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/fib.h"

static inline seq_t triangular_idx(idx_t r)
{
    return (seq_t)r * (r + 1) / 2;
}

seq_t a(idx_t n)
{
    /* Find the row r containing the n‑th element */
    idx_t low = 1, high = 1;
    while (triangular_idx(high) < n)
    {
        high <<= 1;   /* double until we pass n */
    }
    while (low < high)
    {
        idx_t mid = low + ((high - low) >> 1);
        if (triangular_idx(mid) >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;

    /* Compute the offset within that row: k = n - T_{r-1} */
    idx_t prev = (r - 1) * r / 2;          /* T_{r-1} */
    idx_t k = n - prev;

    /* Value = Fibonacci(r) + (k - 1) */
    seq_t fr = fib(r);
    return fr + (seq_t)(k - 1);
}
