/*
 * A139464  a(n) = n! + 2n - 1.
 * https://oeis.org/A139464
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n! + 2*n - 1
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    /* a(1) is the first term; behaviour for n < 1 is undefined. */
    if (n == 0)
        return (seq_t)-1;  /* silence any out‑of‑range call */

    /* Compute n! via the provided helper. */
    seq_t fact = factorial(n);

    /* 2*n + (n! - 1) */
    seq_t two_n = ((seq_t)n) << 1;   /* 2 * n */
    seq_t result = fact + two_n - (seq_t)1;

    return result;
}
