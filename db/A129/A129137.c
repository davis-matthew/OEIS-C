/*
 * A129137  Number of trees on [n], rooted at 1, in which 2 is a descendant of
 *          3.
 * https://oeis.org/A129137
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0}^{n-3} (n-3)! * n^k / k! for n ≥ 1
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 17, u128 n <= 28,
 *            big any n
 * Validated: standard, pass, 35 terms through n = 49 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

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
    if (n < 3)               /* no trees for n = 1,2, below the offset */
        return 0;

    idx_t m = n - 3;         /* m = n-3, upper limit of k */
    seq_t fact = factorial(m);   /* (n-3)! */
    seq_t mult = fact;          /* will hold (n-3)! / k!   */
    seq_t power = 1;            /* n^k                   */
    seq_t sum   = 0;            /* accumulator            */

    for (idx_t k = 0; k <= m; ++k) {
        sum += mult * power;        /* add term for current k */

        if (k < m) {                /* prepare next term   */
            mult /= (seq_t)(k + 1);  /* exact division by (k+1)  */
            power *= (seq_t)n;      /* multiply by n   */
        }
    }
    return sum;
}
