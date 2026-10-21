/*
 * A080382  Triangle read by rows: T(n,k) =
 *          C(n,floor(n/2))/gcd(C(n,floor(n/2)),C(n,k)), k=0..n; central
 *          binomial coefficient is divided by greatest common divisor of
 *          binomial coefficients and corresponding central binomial
 *          coefficient.
 * https://oeis.org/A080382
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = C(r, ⌊r/2⌋) / gcd(C(r, ⌊r/2⌋), C(r, c)) where r and c are
 *            the row and column indices of the n‑th term in the triangle read
 *            by rows (starting with row 0)
 * Range:     u8 n <= 36, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   binomial, gcd, int_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/int_sqrt.h"   /* idx_t int_sqrt(idx_t n); */
#include "../utils/gcd.h"        /* seq_t gcd(seq_t a, seq_t b); */
#include "../utils/binomial.h"   /* seq_t binomial(idx_t n, idx_t k); */

seq_t a(idx_t n)
{
    /* Find the row index r such that the n‑th term lies in that row.
       Let r' = r+1; then r' satisfies r'(r'+1)/2 ≥ n and
       r'(r'+1)/2 < n is false for r'-1.  Compute r' with a
       integer square root and adjust upward if necessary. */
    idx_t t = 8 * n + 1;                      /* 8*n+1 fits in 128‑bit */
    idx_t s = int_sqrt(t);
    idx_t r_prime = (s - 1) / 2;              /* initial guess for r' */

    /* Correct r' upward while the cumulative count is still < n. */
    while ((__uint128_t)r_prime * (r_prime + 1) / 2 < n)
        r_prime++;

    idx_t r = r_prime - 1;                   /* actual row index (0‑based) */

    /* Compute the column index c within the found row. */
    idx_t prev_sum = (__uint128_t)r_prime * (r_prime - 1) / 2; /* terms before this row */
    idx_t c = n - prev_sum - 1;              /* 0‑based column index */

    /* Compute the triangle entry T(r,c). */
    seq_t central = binomial(r, r / 2);
    seq_t k_binom = binomial(r, c);
    seq_t g = gcd(central, k_binom);
    return central / g;                       /* division is exact */
}
