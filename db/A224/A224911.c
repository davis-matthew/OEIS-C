/*
 * A224911  Greatest prime dividing A190339(n).
 * https://oeis.org/A224911
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = greatest prime ≤ 2n+2 (by Bertrand's postulate)
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime_big.h"

/* Greatest prime dividing A190339(n): A060308(n+1) == largest prime <= 2(n+1) */
seq_t a(idx_t n)
{
    /* compute the upper bound 2*(n+1) using seq_t arithmetic */
    seq_t limit = 2 * ((seq_t)n + 1);
    seq_t p = limit;

    /* iterate downward until a prime is found */
    while (true) {
        if (is_prime_big(p))
            return p;
        /* Because n >= 0, limit >= 2, so we will never underflow past 2 */
        if (p <= 2)
            break;          /* safety; should never reach here */
        --p;
    }
    /* fallback: 2 is always prime, but we should never reach this point */
    return 2;
}
