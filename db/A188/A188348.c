/*
 * A188348  Distance between n and the next number with the same number of
 *          distinct prime factors.
 * https://oeis.org/A188348
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = min d>0 such that count_distinct_primes(n+d) =
 *            count_distinct_primes(n)
 * Range:     u8 n <= 2309, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* For uint64_t */
#include <stdbool.h>  /* For bool */

static uint64_t _count_distinct_primes(uint64_t m)
{
    uint64_t cnt = 0;
    if (m < 2) return 0;
    /* Count factor 2 */
    if (m % 2 == 0) {
        cnt++;
        while (m % 2 == 0) m /= 2;
    }
    /* Count odd factors */
    for (uint64_t i = 3; i * i <= m; i += 2) {
        if (m % i == 0) {
            cnt++;
            while (m % i == 0) m /= i;
        }
    }
    if (m > 1) cnt++;
    return cnt;
}

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 2. Behaviour for n < 2 is undefined. */
    uint64_t m = (uint64_t)n;
    uint64_t target = _count_distinct_primes(m);
    uint64_t d = 1;
    while (true) {
        if (_count_distinct_primes(m + d) == target) {
            return (seq_t)d;
        }
        d++;
    }
}
