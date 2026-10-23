/*
 * A115346  Triangle read by rows: T(n,k) = Fibonacci(k+2) - 1.
 * https://oeis.org/A115346
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy, less
 * Formula:   a(n) = Fib(k+2) − 1, where k = n − r(r+1)/2 and r =
 *            floor((sqrt(1+8n)−1)/2) is the row number
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"
#include <stdint.h>

/* Find the row index r for a given flattened index n.
   r satisfies r(r+1)/2 <= n < (r+1)(r+2)/2. */
static idx_t find_row(idx_t n)
{
    idx_t low = 0, high = n;          /* high never underflows for n <= UINT64_MAX */
    while (low < high) {
        idx_t mid = low + (high - low + 1) / 2;      /* upper mid */
        /* Compare mid*(mid+1)/2 <= n  using 128‑bit arithmetic to avoid overflow. */
        if ((__uint128_t)mid * (mid + 1) <= (__uint128_t)2 * n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    idx_t r  = find_row(n);
    /* triangular number of row r */
    idx_t tri = (idx_t)(((__uint128_t)r * (r + 1)) / 2);
    idx_t k   = n - tri;                    /* column index within the row */

    /* Fibonacci(k+2) - 1 */
    seq_t fib_val = fib(k + 2);
    return fib_val - (seq_t)1;
}
