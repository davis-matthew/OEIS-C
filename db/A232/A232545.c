/*
 * A232545  Number of Euler tours of the complete digraph on n vertices.
 * https://oeis.org/A232545
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, walk
 * Formula:   a(n) = n^(n-2)*((n-2)!)^n
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 7, u128 n <= 8,
 *            big any n
 * Validated: standard, pass, 26 terms through n = 30 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

/* Helper: compute factorials iteratively.
   For built‑in integer types the result may overflow for large n,
   but the tests use only n for which the value fits in seq_t. */
static seq_t factorial(idx_t n)
{
    seq_t result = 1;
    for (idx_t i = 2; i <= n; ++i)
        result *= (seq_t)i;
    return result;
}

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;  /* undefined for n < 2 */

    seq_t pow_part   = ipow((seq_t)n, (seq_t)(n - 2));   /* n^(n-2)     */
    seq_t fact       = factorial(n - 2);                     /* (n-2)!      */
    seq_t fact_pow   = ipow(fact, (seq_t)n);              /* (n-2)!^n    */
    return pow_part * fact_pow;                             /* result      */
}
