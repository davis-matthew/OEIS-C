/*
 * A087555  a(n) = largest prime that is the concatenation of two numbers
 *          whose sum is n.
 * https://oeis.org/A087555
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  base, nonn, look
 * Formula:   a(n) = largest prime that is the concatenation of two positive
 *            integers whose sum is n (n ≥ 2). If no such prime exists, the
 *            value 0 is returned
 * Range:     u8 n <= 13, u16 n <= 168, u32 n <= 1711, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   concat_parts, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/concat_parts.h"

seq_t a(idx_t n)
{
    if (n < 2) return 0;
    seq_t best = 0;
    /* Consider all ordered pairs (i, n-i), i = 1 .. n-1 */
    for (idx_t i = 1; i < n; ++i) {
        idx_t j = n - i;
        seq_t cand = concat_parts(i, j);
        if (is_prime_big(cand) && cand > best)
            best = cand;
    }
    return best;
}
