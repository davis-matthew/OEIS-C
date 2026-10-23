/*
 * A128618  Triangle read by rows: A128174 * A127647 as infinite lower
 *          triangular matrices.
 * https://oeis.org/A128618
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(r,k) = (1/2)*(1+(-1)^(r+k))*Fib(k); a(n) = T(r,k) with r,k
 *            derived from n
 * Range:     u8 n <= 104, u16 n <= 324, u32 n <= 1175, u64 n <= 4464,
 *            u128 n <= 5050, big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the triangle read by rows of the lower‑triangular matrix with entries
   T(r,k) = Fib(k) if r and k have the same parity, otherwise 0. */

#include <stdint.h>
#include "../utils/fib.h"

static idx_t _row_from_index(idx_t n) {
    /* Finds the smallest r such that r*(r+1)/2 >= n. */
    idx_t low = 1, high = 2;
    while (( (__uint128_t)high * (high + 1) / 2) < n) {
        high <<= 1;
    }
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (( (__uint128_t)mid * (mid + 1) / 2) >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low; /* r */
}

seq_t a(idx_t n) {
    if (n == 0) return 0;             /* offset starts at 1, but guard anyway */
    idx_t r = _row_from_index(n);
    idx_t prev_tri = (idx_t)((__uint128_t)(r - 1) * r / 2); /* (r-1)*r/2 */
    idx_t k = n - prev_tri;           /* column index (1‑based) */

    /* If r and k have different parity, the entry is 0. */
    if (((r + k) & 1) == 1)
        return 0;

    /* Otherwise return the kth Fibonacci number. */
    return fib(k);
}
