/*
 * A059957  Sum of number of distinct prime factors of n and n+1, or number of
 *          distinct prime factors of n(n+1) or of lcm(n,n+1).
 * https://oeis.org/A059957
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A001221(n) + A001221(n+1)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_distinct_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* number of distinct prime factors of n plus those of n+1 */

#include "../utils/count_distinct_primes.h"

seq_t a(idx_t n)
{
    /* compute distinct prime factors of n and n+1 */
    int cnt_n  = count_distinct_primes((uint64_t)n);
    int cnt_np = count_distinct_primes((uint64_t)(n + 1));
    return (seq_t)(cnt_n + cnt_np);
}
