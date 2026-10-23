/*
 * A173447  The number of iterations for the map r -> A061602(r), starting
 *          with n, needed to reach the end of the first cycle.
 * https://oeis.org/A173447
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed using Floyd's cycle detection on the digit factorial map.
   Sequence a(n) counts the iterations of r -> sum of factorials of
   decimal digits before the first repeated value occurs. */
#include <stdint.h>

static const uint32_t fact_digit[10] = {
    1, 1, 2, 6, 24, 120, 720, 5040, 40320, 362880
};

static inline uint64_t digit_fact_sum_uint64(uint64_t x)
{
    if (x == 0) return 1ULL;          /* 0! = 1 */
    uint64_t sum = 0;
    while (x != 0) {
        sum += fact_digit[x % 10];
        x /= 10;
    }
    return sum;
}

static inline uint64_t map_r(uint64_t r)
{
    return digit_fact_sum_uint64(r);
}

seq_t a(idx_t n)
{
    uint64_t r = (uint64_t)n;

    /* Floyd's algorithm to locate the cycle */
    uint64_t tortoise = map_r(r);
    uint64_t hare     = map_r(map_r(r));
    while (tortoise != hare) {
        tortoise = map_r(tortoise);
        hare = map_r(map_r(hare));
    }

    /* Find the start of the cycle (mu) */
    uint64_t mu = 0;
    tortoise = r;
    while (tortoise != hare) {
        tortoise = map_r(tortoise);
        hare = map_r(hare);
        ++mu;
    }

    /* Find the cycle length (lambda) */
    uint64_t lambda = 1;
    hare = map_r(tortoise);
    while (tortoise != hare) {
        hare = map_r(hare);
        ++lambda;
    }

    /* Number of distinct terms before a repeat: mu + lambda - 1 */
    return (seq_t)(mu + lambda - 1);
}
