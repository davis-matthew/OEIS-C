/*
 * A109496  a(n) is the first integer k for which b(b(...b(n)...))=5, where
 *          the transformation b(m), the m-th term of A008474, is taken k-1
 *          times.
 * https://oeis.org/A109496
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = minimal k such that b^{k-1}(n)=5, where * b(m) = (sum of
 *            distinct prime factors of m) + (sum of their exponents)
 * Range:     u8 n <= 109, u16 n <= 109, u32 n <= 109, u64 n <= 109,
 *            u128 n <= 109, big any n
 * Helpers:   b_func
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/b_func.h"

seq_t a(idx_t n)
{
    /* For n=5 the starting value already equals 5. */
    if ((seq_t)n == 5)
        return (seq_t)1;

    seq_t cur = (seq_t)n;
    idx_t steps = 0;

    /* Apply b repeatedly until the value 5 is reached. */
    while (cur != (seq_t)5)
    {
        cur = b_func(cur);
        ++steps;
        /* Safeguard: if the chain grows too long we bail out. */
        if (steps > 1000000)
            break; /* should not happen for the intended range */
    }

    return (seq_t)(steps + 1);
}
