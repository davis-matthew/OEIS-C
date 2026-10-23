/*
 * A173076  Triangle T(n, k, q) = binomial(n, k) - 1 +
 *          q^(floor(n/2))*binomial(n-2, k-1) with T(n, 0, q) = T(n, n, q) = 1
 *          and q = 2, read by rows.
 * https://oeis.org/A173076
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the (n)-th element of the triangle T defined by T(n, k)
 *            = binomial(n, k) - 1 + 2^⌊n/2⌋ * binomial(n-2, k-1) with T(n, 0)
 *            = T(n, n) = 1, and a(n) is obtained by writing the rows of T one
 *            after another (row 0 first, then row 1, etc.)
 * Range:     u8 n <= 38, u16 n <= 110, u32 n <= 335, u64 n <= 1147,
 *            u128 n <= 1325, big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n) {
    /* Find the row r such that r(r+1)/2 <= n < (r+1)(r+2)/2. */
    idx_t r = 0;
    while ((r + 1ULL) * (r + 2ULL) / 2ULL <= n)
        ++r;

    /* Column within the row. */
    idx_t col = n - r * (r + 1ULL) / 2ULL;

    /* Boundary elements are 1. */
    if (col == 0 || col == r)
        return (seq_t)1;

    /* For r < 2 the interior does not exist; return 1. */
    if (r < 2)
        return (seq_t)1;

    /* Compute the interior value. */
    seq_t choose_rc   = binomial(r, col);
    seq_t choose_down = binomial(r - 2, col - 1);
    seq_t pow_part    = pow2(r / 2ULL);          /* 2^⌊r/2⌋ */
    seq_t mult        = pow_part * choose_down; /* term q^floor(n/2)*binom(r-2,col-1) */
    seq_t result      = choose_rc - 1 + mult;   /* binom(r,col)-1 + that term */

    return result;
}
