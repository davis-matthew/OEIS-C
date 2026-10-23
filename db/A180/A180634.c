/*
 * A180634  Numbers n such that the discriminant of the n-th cyclotomic
 *          polynomial is a square.
 * https://oeis.org/A180634
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 155, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A180634 – nth positive integer whose product of units modulo n is 1
   (equivalently: n ≠ 4, n is not an odd prime power, and n is not twice an odd prime power). */

#include <stdbool.h>
#include <stdint.h>

/* Return true if n is an odd prime power p^k with p odd and k ≥ 1. */
static bool is_odd_prime_power(uint64_t n)
{
    if (n <= 2)        return false;   /* 1 and 2 are not odd prime powers */
    if ((n & 1ULL) == 0) return false; /* even numbers cannot be odd prime powers */

    uint64_t temp = n;
    uint64_t spf = 0;                   /* smallest odd prime factor */
    for (uint64_t i = 3; i <= temp / i; i += 2)
    {
        if (temp % i == 0) { spf = i; break; }
    }

    if (spf == 0)        return true;   /* n itself is an odd prime (k = 1) */

    /* divide n repeatedly by spf and check that no other prime factor remains */
    while (temp % spf == 0) temp /= spf;
    return (temp == 1);
}

/* Return true if n should be excluded from the sequence. */
static bool is_excluded(uint64_t n)
{
    if (n == 4) return true;                 /* special case */

    /* n itself is an odd prime power */
    if (is_odd_prime_power(n)) return true;

    /* n is twice an odd prime power */
    if ((n & 1ULL) == 0)                     /* even */
    {
        uint64_t m = n >> 1;
        if (is_odd_prime_power(m)) return true;
    }

    return false;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;            /* undefined for index 0 */

    idx_t count = 0;
    uint64_t cand  = 0;

    while (count < n)
    {
        ++cand;
        if (!is_excluded(cand)) ++count;
    }

    return (seq_t)cand;
}
