/*
 * A109427  Numbers n such that sigma(n)/omega(n) is an integer [sigma(n) =
 *          sum of divisors of n; omega(n) = number of distinct prime factors
 *          of n].
 * https://oeis.org/A109427
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 237, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 88 terms through n = 9512 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A109427 – numbers n for which sigma(n)/omega(n) is an integer. 
   The function iterates over candidates n = 2,3,… and counts those
   satisfying the condition, until the nth term is found. */
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/* helper: sum of divisors of an unsigned integer x (x >= 1) */
static seq_t sum_divisors(idx_t x)
{
    seq_t sum = 1;             /* include divisor 1 */
    idx_t i;
    for (i = 2; i * i <= x; ++i)
    {
        if (x % i == 0)
        {
            sum += i;
            idx_t other = x / i;
            if (other != i) sum += other;
        }
    }
    if (x > 1) sum += x;       /* include x itself */
    return sum;
}

/* count distinct prime factors of x (omega function) */
static size_t count_distinct_prime_factors(uint64_t x)
{
    size_t cnt = 0;
    if (x < 2)
        return 0;
    if ((x & 1ULL) == 0) {
        ++cnt;
        do { x >>= 1; } while ((x & 1ULL) == 0);
    }
    for (uint64_t p = 3; p * p <= x; p += 2) {
        if (x % p == 0) {
            ++cnt;
            do { x /= p; } while (x % p == 0);
        }
    }
    if (x > 1)
        ++cnt;
    return cnt;
}

seq_t a(idx_t n)
{
    idx_t found = 0;          /* number of valid terms seen so far */
    for (idx_t num = 2; ; ++num) {
        seq_t sigma = sum_divisors(num);                 /* sigma(n) */
        idx_t omega = count_distinct_prime_factors(num); /* omega(n) */
        if (omega != 0 && sigma % (seq_t)omega == 0) {    /* condition holds */
            ++found;
            if (found == n)
                return (seq_t)num;                      /* return nth term */
        }
    }
}
