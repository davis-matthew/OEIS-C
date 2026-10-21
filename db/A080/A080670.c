/*
 * A080670  Literal reading of the prime factorization of n.
 * https://oeis.org/A080670
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look, easy
 * Formula:   a(n) is the decimal number obtained by concatenating the prime
 *            factors of n in increasing order, appending each exponent only
 *            if it is greater than 1
 * Range:     u8 n <= 32, u16 n <= 395, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   concat_with_num, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/concat_with_num.h"

seq_t a(idx_t n)
{
    /* a(1) is defined to be 1. */
    if (n == 1)
        return (seq_t)1;

    /* Factor n with the provided utility. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t   cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* Build the concatenated number. */
    seq_t res = 0;
    for (size_t i = 0; i < cnt; ++i) {
        res = concat_with_num(res, primes[i]);     /* prime */
        if (exps[i] > 1)
            res = concat_with_num(res, exps[i]);   /* exponent if >1 */
    }
    return res;
}
