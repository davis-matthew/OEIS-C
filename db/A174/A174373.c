/*
 * A174373  Triangle T(n,m) = nextprime(binomial(n,m)) read by rows.
 * https://oeis.org/A174373
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, less
 * Formula:   a(n) = nextprime( binomial( row(n), col(n) ) ) where row(n) and
 *            col(n) satisfy n = row(row+1)/2 + col , 0 ≤ col ≤ row
 * Range:     u8 n <= 39, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   binomial, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* external utilities */
#include <math.h>
#include <stdbool.h>
#include "../utils/binomial.h"
#include "../utils/is_prime_big.h"

/* Find the smallest prime ≥ x (for arbitrary‑precision seq_t). */
static inline seq_t next_prime_big(seq_t x)
{
    seq_t cand = x;
    while (!is_prime_big(cand))
        ++cand;
    return cand;
}

seq_t a(idx_t n)
{
    /* locate the row containing “n” */
    idx_t r = (idx_t)((sqrt(8.0 * (double)n + 1.0) - 1.0) / 2.0);

    /* correct possible rounding errors */
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;
    while (r * (r + 1) / 2 > n)
        --r;

    idx_t start = r * (r + 1) / 2;
    idx_t m = n - start;          /* column in this row   */

    seq_t b = binomial(r, m);      /* C(r,m)                     */
    return next_prime_big(b);      /* smallest prime ≥ C(r,m)    */
}
