/*
 * A062071  a(n) = [n/1] + [n/(2^2)] + [n/(3^3)] + [n/(4^4)] + ... + [n/(k^k)]
 *          + ..., up to infinity, where [ ] is the floor function.
 * https://oeis.org/A062071
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ∑_{i=1..∞} ⌊ n / i^i ⌋
 * Range:     u8 n <= 199, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function implements the definition directly.  It stops once
   i^i exceeds n, so the number of iterations is O(log n) and
   negligible for typical values.  All arithmetic is performed in the
   types required by the build (idx_t and seq_t).  An unsigned
   __int128 is used only for computing i^i safely; the result is
   promoted to seq_t before being used in the division.  No
   pre‑computed tables are required. */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t nn   = (seq_t)n;          /* convert n once */
    for (idx_t i = 1; ; ++i)
    {
        /* compute i^i, stop early if it exceeds n */
        unsigned __int128 pow = 1;
        for (idx_t j = 0; j < i; ++j)
        {
            pow *= (unsigned __int128)i;
            if (pow > nn || pow == 0) /* pow==0 handles wrap‑around internally */
                break;
        }
        if (pow > nn || pow == 0)
            break;

        seq_t denom = (seq_t)pow;   /* safe conversion: pow <= n */
        sum += nn / denom;
    }
    return sum;
}
