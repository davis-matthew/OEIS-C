/*
 * A008887  Aliquot sequence starting at 60.
 * https://oeis.org/A008887
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 11, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 60,
   a(n+1) = A001065(a(n)) = sigma(a(n)) – a(n),
   where sigma(x) is the sum of all positive divisors of x (including x). */

#include <stdint.h>

/* Sum of proper divisors of a 64‑bit integer (x > 0).
   For x == 0 the aliquot part is defined to be 0. */
static seq_t aliquot_part(uint64_t x)
{
    if (x == 0) return 0;

    uint64_t sum = 1;          /* 1 is a divisor of every x > 0 */
    uint64_t remaining = x;
    /* factorization and sigma computation */
    for (uint64_t p = 2; p * p <= remaining; ++p) {
        if (remaining % p == 0) {
            uint64_t power = p;
            uint64_t term = 1 + p;
            remaining /= p;
            while (remaining % p == 0) {
                power *= p;
                term += power;
                remaining /= p;
            }
            sum *= term;
        }
    }
    if (remaining > 1) {       /* remaining is a prime factor */
        sum *= (1 + remaining);
    }
    return (seq_t)(sum - x);   /* proper divisors only */
}

seq_t a(idx_t n)
{
    seq_t cur = 60;                /* a(0) */
    if (n == 0) return cur;

    for (idx_t i = 0; i < n; ++i) {
        cur = aliquot_part((uint64_t)cur);
        if ((uint64_t)cur == 0) break; /* sequence terminates */
    }
    return cur;
}
