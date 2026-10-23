/*
 * A039790  Prime numbers prefixed with a '1'.
 * https://oeis.org/A039790
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, dumb
 * Formula:   a(n) = concat(1, prime(n))
 * Range:     u8 n <= 25, u16 n <= 1229, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   concat_parts, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This uses the provided helper functions get_prime and concat_parts. */

#include "../utils/get_prime.h"
#include "../utils/concat_parts.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                       /* offset is 1, n == 0 is unused */

    return concat_parts(1, (idx_t)get_prime(n));
}
