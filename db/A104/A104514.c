/*
 * A104514  a(n) = least number k > 1 of consecutive integers which sum to
 *          2*n; or a(n) = 0 if n is a power of 2.
 * https://oeis.org/A104514
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least number k>1 of consecutive integers that sum to 2*n;
 *            if n is a power of 2 then a(n) = 0. Equivalent implementation:
 *            a(n) is the smallest divisor d>1 of 4*n such that either d or
 *            (4*n)/d is odd
 * Range:     u8 n <= 65, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* 0 for power‑of‑two n  */
    if ((n & (n - 1)) == 0)
        return 0;

    /* 4 * n (stored in seq_t to avoid overflow when seq_t is larger than idx_t) */
    seq_t val = (seq_t)n * (seq_t)4;

    /* search the smallest divisor d>1 of val with the required parity property */
    for (seq_t d = 2; d * d <= val; ++d)
    {
        if (val % d == 0)
        {
            seq_t complement = val / d;
            if ((d & 1) || (complement & 1))
                return d;
        }
    }

    /* In theory we should never reach here because n is not a power of two,
       but return val as a fallback (val itself is a divisor satisfying the
       condition) */
    return val;
}
