/*
 * A049852  Concatenate "n" and "nextprime(n)".
 * https://oeis.org/A049852
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = concatenate_decimal(n, next_prime_gt(n)), where
 *            next_prime_gt(n) is the smallest prime strictly greater than n.
 *            The function computes it for any n ≥ 1
 * Range:     u8 n <= 6, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   digit_len, is_prime, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/digit_len.h"
#include "../utils/pow10.h"

static uint64_t next_prime_gt(uint64_t n)
{
    uint64_t p = n + 1;
    while (!is_prime((idx_t)p))
        ++p;
    return p;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;  /* sequence offset is 1 */

    uint64_t np = next_prime_gt((uint64_t)n);   /* next prime > n */

    seq_t dn   = (seq_t)n;          /* decimal representation of n */
    seq_t np_t = (seq_t)np;         /* next prime as seq_t */

    /* compute 10^(digit_count(np)) */
    idx_t exp = (idx_t)digit_len(np_t);   /* digit count of np */
    seq_t pow10_exp = pow10(exp);

    /* concatenate: n * 10^d + nextprime(n) */
    return dn * pow10_exp + np_t;
}
