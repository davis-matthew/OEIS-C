/*
 * A090302  Begin with n and consider numbers obtained by successively
 *          subtracting 0, 1, 2, 3, ...; a(n) = largest prime that arises in
 *          the process, i.e., largest prime of the form n - T(r), where T(r)
 *          is the r-th triangular number; or 0 if no such number exists.
 * https://oeis.org/A090302
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = maximum prime that equals n - r(r+1)/2 for some
 *            non‑negative integer r, or 0 if no such prime exists
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    seq_t best = 0;          /* best prime found so far (0 means none yet) */
    idx_t  triangular = 0;   /* current triangular number T(r) = r(r+1)/2 */

    /* iterate over r = 0, 1, 2, ... until T(r) exceeds n */
    for (idx_t r = 0; /* empty */; ++r) {
        if (triangular > n) break;                 /* no further candidates */

        seq_t cand = (seq_t)(n - triangular);       /* candidate n - T(r) */

        if (cand >= 2 && is_prime_big(cand)) {
            if (cand > best) best = cand;           /* keep the largest prime */
        }

        triangular += r + 1;                         /* next triangular number */
    }

    return best;
}
