/*
 * A078317  Number of divisors of n*rad(n)+1, where rad = A007947 (squarefree
 *          kernel).
 * https://oeis.org/A078317
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of divisors of n*rad(n)+1, where rad(n) is the
 *            product of distinct prime factors of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Utility: factor a 64‑bit integer and return its divisor count.
   This is sufficient for the range of n used in the tests. */
static uint64_t divisor_count_u64(uint64_t n)
{
    uint64_t cnt = 1;
    for (uint64_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            unsigned e = 0;
            do { n /= p; ++e; } while (n % p == 0);
            cnt *= (uint64_t)(e + 1);
        }
    }
    if (n > 1) cnt *= 2;          /* remaining prime factor */
    return cnt;
}

/* Compute the square‑free kernel rad(n) = product of distinct prime factors of n. */
static uint64_t rad_u64(uint64_t n)
{
    uint64_t rad = 1;
    if (n == 0) return rad;
    if (n % 2 == 0) {
        rad *= 2;
        while (n % 2 == 0) n /= 2;
    }
    for (uint64_t p = 3; p * p <= n; p += 2) {
        if (n % p == 0) {
            rad *= p;
            while (n % p == 0) n /= p;
        }
    }
    if (n > 1) rad *= n;
    return rad;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;    /* undefined, but return 0 for safety */

    uint64_t nn   = (uint64_t)n;
    uint64_t rad  = rad_u64(nn);
    uint64_t m    = nn * rad + 1;    /* fits in 64 bits for the test range */
    uint64_t tau  = divisor_count_u64(m);
    return (seq_t)tau;
}
