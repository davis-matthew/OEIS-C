/*
 * A089777  a(n) = smallest prime of the form n followed by a prime.
 * https://oeis.org/A089777
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = smallest prime of the form n followed by a prime
 * Range:     u8 n <= 13, u16 n <= 657, u32 n <= 234, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   concat_parts, is_prime_big, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_small.h"
#include "../utils/is_prime_big.h"
#include "../utils/concat_parts.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;            /* offset is 1; return 0 for invalid indices */

    seq_t nseq = (seq_t)n;
    for (uint64_t p = 2; ; ++p)
    {
        if (!is_prime_small((seq_t)p))
            continue;

        seq_t cand = concat_parts((idx_t)n, (idx_t)p);
        if (is_prime_big(cand))
            return cand;
    }
}
