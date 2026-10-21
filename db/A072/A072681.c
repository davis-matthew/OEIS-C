/*
 * A072681  a(n) = (n - A007917(n)) * (A007918(n) - n).
 * https://oeis.org/A072681
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, nice, look
 * Formula:   a(n) = (n - A007917(n)) * (A007918(n) - n)
 * Range:     u8 n <= 1338, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   my_is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/my_is_prime.h"

/* Largest prime ≤ n.  Integrity of n ≥ 2 is assumed by the caller. */
static uint64_t prev_prime(idx_t n)
{
    uint64_t x = (uint64_t)n;
    if (my_is_prime(x)) return x;
    if (x % 2 == 0) x--;            /* make x odd */
    while (x >= 2) {
        if (my_is_prime(x)) return x;
        x -= 2;
    }
    return 2;                        /* should not happen for n≥2 */
}

/* Smallest prime ≥ n.  Integrity of n ≥ 2 is assumed by the caller. */
static uint64_t next_prime(idx_t n)
{
    uint64_t x = (uint64_t)n;
    if (my_is_prime(x)) return x;
    if (x % 2 == 0) x++;            /* make x odd */
    while (1) {
        if (my_is_prime(x)) return x;
        x += 2;
    }
}

seq_t a(idx_t n)
{
    if (n < 2) return 0;            /* offset starts at 2; unused values are irrelevant */
    uint64_t up = prev_prime(n);    /* A007917(n) */
    uint64_t dn = next_prime(n);    /* A007918(n) */
    uint64_t d1 = (uint64_t)n - up;
    uint64_t d2 = dn - (uint64_t)n;
    return (seq_t)(d1 * d2);
}
