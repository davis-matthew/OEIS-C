/*
 * A094443  Triangular array T(n,k) = Fibonacci(n+3-k)*C(n,k), k=0..n, n>=0.
 * https://oeis.org/A094443
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,c) where * T(r,c) = binomial(r,c) * fib(r + 3 - c) *
 *            (r,c) is obtained by unfolding the triangular array row‑by‑row.
 *            * Row r starts at index r*(r+1)/2, and column c = n - r*(r+1)/2
 * Range:     u8 n <= 29, u16 n <= 93, u32 n <= 331, u64 n <= 1191,
 *            u128 n <= 4589, big any n
 * Helpers:   binomial, fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/fib.h"

static idx_t row_from_index(idx_t n)
{
    /* Find the largest r such that r*(r+1)/2 <= n.
       Binary search works because r*(r+1)/2 grows monotonically. */
    idx_t low = 0, high = 1;
    while ( (unsigned __int128)high * (high + 1) / 2 <= n )
        high <<= 1;
    while (low + 1 < high) {
        idx_t mid = low + (high - low) / 2;
        if ( (unsigned __int128)mid * (mid + 1) / 2 <= n )
            low = mid;
        else
            high = mid;
    }
    return low;
}

extern seq_t a(idx_t n)
{
    /* Determine (r,c) from the linear index n. */
    idx_t r = row_from_index(n);
    idx_t pre = (unsigned __int128)r * (r + 1) / 2;   /* elements before row r */
    idx_t c = n - pre;                                /* column in row r */

    /* Compute the term T(r,c). */
    seq_t bin = binomial(r, c);
    seq_t fib_val = fib(r + 3 - c);
    return bin * fib_val;
}
