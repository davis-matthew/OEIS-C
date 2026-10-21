/*
 * A062509  a(n) = n^omega(n).
 * https://oeis.org/A062509
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^omega(n)
 * Range:     u8 n <= 17, u16 n <= 41, u32 n <= 329, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   big_pow, count_distinct_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/count_distinct_primes.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0)          /* offset 1, but guard against 0 */
        return 0;

    int k = count_distinct_primes((uint64_t)n); /* number of distinct primes of n */
    seq_t base = (seq_t)n;
    seq_t exp  = (seq_t)k;

    return big_pow(base, exp);  /* compute n^k */
}
