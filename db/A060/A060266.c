/*
 * A060266  Difference between 2n and the following prime.
 * https://oeis.org/A060266
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nextprime(2·n) – 2·n (the smallest prime strictly greater
 *            than 2n minus 2n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Simple deterministic prime test for 64‑bit numbers
   (suitable for values up to a few million). */
static bool is_prime_u64(uint64_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    if (n % 3 == 0) return n == 3;
    uint64_t i = 5;
    uint64_t step = 2;
    while (i <= n / i)
    {
        if (n % i == 0) return false;
        i += step;
        step = 6 - step;   /* 5,7,11,13,17,19,... */
    }
    return true;
}

/* External‐linkage function requested by the framework. */
seq_t a(idx_t n)
{
    /* 2*n may overflow seq_t but is stored in uint64_t for safety. */
    uint64_t twice = (uint64_t)n * 2;
    uint64_t p = twice + 1;              /* candidate prime > 2n        */
    while (!is_prime_u64(p)) ++p;        /* find next prime              */
    return (seq_t)(p - twice);           /* difference                    */
}
