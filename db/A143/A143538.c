/*
 * A143538  Triangle read by rows, T(n,k) = 1 if k is prime, 0 otherwise; 1 <=
 *          k <= n.
 * https://oeis.org/A143538
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 1 if the column index k of n in the triangle T(r,k) (read
 *            row‑wise, r = 1,2,…) is prime, otherwise 0. The indices are
 *            flattened by row: a(1) = T(1,1), a(2) = T(2,1), a(3) = T(2,2),
 *            a(4) = T(3,1), …
 * Range:     u8 n <= 5050, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"   /* bool is_prime(idx_t x); */

/* Find the row r satisfying r*(r+1)/2 >= n .
   Uses a binary search on the range 0 … 2^32 to avoid overflow
   for uint64_t indices. */
static idx_t
row_from_n(idx_t n)
{
    if (n == 0) return 0;          /* undefined, but keeps type safety */

    idx_t low = 0;
    idx_t high = 1;

    /* Exponentially increase high until it is large enough.
       The product high*(high+1)/2 is computed in 128‑bit to avoid overflow. */
    while (( (__int128)high * (high + 1) ) / 2 < n) {
        high <<= 1;
        if (high == 0) {          /* wrap‑around protection, should not happen */
            high = UINT64_MAX;
            break;
        }
    }

    /* Binary search for minimal high with high*(high+1)/2 >= n. */
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        __int128 tri = (__int128)mid * (mid + 1) / 2;
        if (tri < n)
            low = mid + 1;
        else
            high = mid;
    }
    return high;   /* minimal r with triangular number >= n */
}

/* External function required by the build system. */
seq_t
a(idx_t n)
{
    /* Determine row r and previous triangular sum. */
    idx_t r    = row_from_n(n);
    idx_t prev = (r - 1) * r / 2;          /* sum of previous rows (safe for idx_t) */
    idx_t k    = n - prev;                 /* column index (1‑based) */

    /* Return 1 if k is prime, else 0. */
    return is_prime(k) ? (seq_t)1 : (seq_t)0;
}
