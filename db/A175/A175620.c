/*
 * A175620  Triangle read by rows: T(n,k) = 2^(prime(n) - k - 1) mod n, 1 <= k
 *          <= n.
 * https://oeis.org/A175620
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the n-th element of the triangle read by rows, where
 *            T(r,k) = 2^(prime(r)-k-1) mod r, 1 ≤ k ≤ r
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Dependencies:  get_prime and mod_pow utilities. */
#include "../utils/get_prime.h"
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    /* The sequence is 1‑based; indices < 1 are treated as 0. */
    if (n == 0) return 0;

    /* Find the row r such that r*(r+1)/2 >= n. */
    idx_t r  = 0;
    idx_t tri = 0;
    while (tri < n)
    {
        ++r;
        tri += r;                /* tri = r(r+1)/2 */
    }

    /* Position k inside that row (1‑based). */
    idx_t prev_tri = tri - r;   /* sum of lengths of rows < r */
    idx_t k = n - prev_tri;     /* 1 ≤ k ≤ r */

    /* Compute the triangle entry: T(r,k). */
    idx_t p   = get_prime(r);          /* prime(r) */
    idx_t exp = p - k - 1;             /* exponent, always ≥ 0 */

    /* 2^(exp) mod r; result fits in seq_t. */
    return mod_pow((seq_t)2, exp, (seq_t)r);
}
