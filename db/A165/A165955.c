/*
 * A165955  n-th odd nonprime plus n-th even nonprime.
 * https://oeis.org/A165955
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A014076(n) + A163300(n)
 * Range:     u8 n <= 45, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Validated: standard, pass, 75 terms through n = 4785 (2026-11-01)
 * Helpers:   is_prime_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_seq.h"

/* Return the n-th odd nonprime (1,9,15,21,25,...) */
static seq_t nth_odd_nonprime(idx_t n)
{
    idx_t count = 0;
    seq_t cand = 1;   /* first odd nonprime */

    while (count < n)
    {
        if (!is_prime_seq(cand))
        {
            ++count;
            if (count == n)
                return cand;
        }
        cand += 2;   /* advance to the next odd number */
    }
    /* The loop always returns; this point is unreachable. */
    return cand;
}

seq_t a(idx_t n)
{
    if (n == 0)          /* the sequence is defined for n ≥ 1 */
        return 0;

    seq_t odd   = nth_odd_nonprime(n);
    seq_t even  = (n == 1) ? 0 : (seq_t)2 * (seq_t)n; /* 0,4,6,8,... */
    return odd + even;
}
