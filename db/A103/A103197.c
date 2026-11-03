/*
 * A103197  Number of compositions of n into Fibonacci number of parts.
 * https://oeis.org/A103197
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Σ_{m∈Fib, m≤n} binom(n−1, m−1) (distinct Fibonacci
 *            numbers)
 * Range:     u8 n <= 10, u16 n <= 19, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Validated: partial, pass, 34 terms through n = 34 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <sys/types.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* a(1) = 1;  for n = 0 (outside the offset) return 0 */
    if (n == 0) return 0;

    seq_t sum = 0;
    idx_t prev = 1;     /* first distinct Fibonacci number */
    idx_t curr = 2;     /* second distinct Fibonacci number */

    /* Add contribution for m = 1 */
    sum += binomial(n - 1, prev - 1);

    /* Add contribution for m = 2, if it does not exceed n */
    if (curr <= n)
        sum += binomial(n - 1, curr - 1);

    /* Build further distinct Fibonacci numbers and add */
    while (1) {
        idx_t next = prev + curr;
        if (next > n) break;
        sum += binomial(n - 1, next - 1);
        prev = curr;
        curr = next;
    }

    return sum;
}
