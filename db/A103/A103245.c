/*
 * A103245  Triangle read by rows: T(n,k) = binomial(2n+1,
 *          n-k)*Fibonacci(2k+1), 0 <= k <= n.
 * https://oeis.org/A103245
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is obtained by flattening the triangle T(r,k) =
 *            binomial(2r+1, r-k) * Fibonacci(2k+1) row by row
 * Range:     u8 n <= 14, u16 n <= 38, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   binom_n_k, fib_u64, row_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/row_of.h"
#include "../utils/binom_n_k.h"
#include "../utils/fib_u64.h"

/* Compute the triangle entry T(r,k). */
static seq_t compute_T(idx_t r, idx_t k)
{
    /* binomial(2r+1, r-k) */
    seq_t bin = binom_n_k(2 * r + 1, r - k);

    /* Fibonacci(2k+1) */
    seq_t f    = fib_u64(2 * k + 1);

    return bin * f;
}

/* Main expose: sequence a(n). */
seq_t a(idx_t n)
{
    /* Find the row r that contains the n‑th flattened entry. */
    idx_t r = row_of(n);

    /* Starting index of this row. */
    idx_t start = r * (r + 1) / 2;

    /* Column index within the row. */
    idx_t k = n - start;

    /* Return the triangle entry T(r,k). */
    return compute_T(r, k);
}
