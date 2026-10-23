/*
 * A008495  Expansion of (1-x^13) / (1-x)^13.
 * https://oeis.org/A008495
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binom(n+12, 12) for 0 <= n < 13 a(n) = binom(n+12, 12) -
 *            binom(n-1, 12) for n >= 13 where binom(a, b) = a! / (b! (a-b)!)
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 27, u64 n <= 219, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute binomial coefficient C(n, k) for n >= k >= 0.
   Uses 128‑bit arithmetic to avoid overflow during intermediate products.
   The result is stored in type seq_t.  The function assumes the final
   value fits in seq_t; intermediate calculations use unsigned __int128 */
static seq_t
binom(idx_t n, idx_t k)
{
    if (k > n)
        return 0;
    if (k == 0 || k == n)
        return 1;
    /* Use symmetry: C(n, k) == C(n, n-k) */
    if (k > n - k)
        k = n - k;

    seq_t result = 1;
    for (idx_t i = 1; i <= k; ++i) {
        /* Multiply by (n - k + i) first, then divide by i */
        unsigned __int128 temp = (unsigned __int128)result *
                                 (unsigned __int128)(n - k + i);
        temp /= i;
        result = (seq_t)temp;
    }
    return result;
}

/* Main sequence function */
seq_t
a(idx_t n)
{
    /* first part: binom(n+12, 12) */
    seq_t top = binom(n + 12, 12);

    /* second part: subtract binom(n-1, 12) only when n >= 13 */
    if (n >= 13) {
        seq_t sub = binom(n - 1, 12);
        /* unsigned subtraction: guaranteed non‑negative */
        return top - sub;
    } else {
        return top;
    }
}
