/*
 * A087558  a(n) = largest prime that is the concatenation of two numbers
 *          whose product is n.
 * https://oeis.org/A087558
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   concat_parts, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A087558:  a(n) = largest prime that is the concatenation of
   two factors of n‑1.  For a given n (n ≥ 2) we find every pair
   (a,b) such that a·b = n‑1, form the two possible concatenations
   a‖b and b‖a, test those for primality, and return the maximum.
   If no such prime exists, 0 is returned. */
#include <stdbool.h>
#include "../utils/is_prime_big.h"   /* bool is_prime_big(seq_t) */
#include "../utils/concat_parts.h"  /* seq_t concat_parts(idx_t a, idx_t b) */

/* External linkage function required by the framework */
seq_t a(idx_t n)
{
    /* a(2) is the first defined term; for n < 2 the value is undefined. */
    if (n < 2)
        return 0;

    idx_t m = n - 1;          /* product of the two factors */
    seq_t best = 0;

    /* Iterate over all divisors of m */
    for (idx_t d = 1; d * d <= m; ++d)
    {
        if (m % d != 0)
            continue;

        idx_t e = m / d;          /* complementary divisor */

        /* Concatenate d and e, then e and d */
        seq_t cand1 = concat_parts(d, e);
        seq_t cand2 = concat_parts(e, d);

        if (is_prime_big(cand1) && cand1 > best)
            best = cand1;
        if (is_prime_big(cand2) && cand2 > best)
            best = cand2;
    }

    return best;
}
