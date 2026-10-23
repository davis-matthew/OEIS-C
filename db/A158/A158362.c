/*
 * A158362  a(n) = binomial((n+1)^3,n+2).
 * https://oeis.org/A158362
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial((n+1)^3, n+2)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 7, u128 n <= 12,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Compute binomial coefficient (N choose k) using a multiplication
 *  algorithm that cancels common factors before each multiplication.
 *  This preserves correctness for arbitrary‑precision seq_t and keeps
 *  intermediary values small for built‑in integer types. */
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;      /* a(0) is defined as 0 in OEIS entry */

    /* (n+1)^3 fits in idx_t for the test suite */
    idx_t m = n + 1;
    idx_t N = m * m * m;   /* N = (n+1)^3 */
    idx_t k = n + 2;       /* k = n+2 */

    if (k > N)
        return 0; /* should not happen */

    /* Use symmetry to minimise the number of steps */
    if (k > N - k)
        k = N - k;

    seq_t res = 1;

    for (idx_t i = 1; i <= k; ++i)
    {
        seq_t num   = (seq_t)(N - k + i); /* numerator term */
        seq_t denom = (seq_t)i;           /* denominator term */

        /* Cancel common factors between res and denom */
        seq_t g = gcd(denom, res);
        denom /= g;
        res   /= g;

        /* Cancel common factors between num and remaining denom */
        g = gcd(num, denom);
        num   /= g;
        denom /= g;      /* should be 1 after this step */

        /* Multiply the remaining numerator into the result */
        res *= num;
    }

    return res;
}
