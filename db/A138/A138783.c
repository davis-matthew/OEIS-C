/*
 * A138783  a(n) = n*(n - 1)*(27*n^2 - 67*n + 74)*n!/24.
 * https://oeis.org/A138783
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n-1)*(27*n^2 - 67*n + 74)*n!/24
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 17,
 *            big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    /* Handle the first three values explicitly – they are not given by
       the formula when n < 4 because of the division by 24. */
    if (n == 1) return 0;
    if (n == 2) return 8;
    if (n == 3) return 174;

    /* n! is used in the formula.  For n >= 4 the value is divisible by 24,
       so dividing by 24 first keeps intermediate values small enough for
       64‑bit arithmetic while still yielding the exact result for all n
       that fit in seq_t. */
    seq_t fact = factorial(n);   /* n! */
    fact /= 24;                  /* divide by 24 (exact for n >= 4) */

    /* Compute the polynomial part 27*n^2 - 67*n + 74. */
    seq_t n_seq = (seq_t)n;
    seq_t poly = (27ULL * n_seq * n_seq) - (67ULL * n_seq) + 74ULL;

    /* Multiply the remaining factors in an order that avoids overflow
       as long as the final result fits in seq_t. */
    seq_t result = fact * (n_seq * (n_seq - 1));
    result *= poly;

    return result;
}
