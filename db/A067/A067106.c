/*
 * A067106  a(n) = smallest number whose digit product equals n!/m where m is
 *          the product of all prime factors > 7 of n! (with multiplicity).
 * https://oeis.org/A067106
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A067106: smallest integer whose decimal digit product
 * equals the 7‑smooth part of n! (i.e. n! divided by all prime
 * factors >7).  The algorithm is:
 *   1. Compute exponents of 2,3,5,7 in n! (Legendre’s formula).
 *   2. Replace these prime factors by decimal digits using a greedy
 *      scheme that prefers larger digits (9,8,7,6,5,4,3,2).
 *   3. Sort the digits ascending to obtain the minimal numeric value.*/

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* ----- Compute exponents of primes ≤ 7 in n! ----- */
    uint64_t e2 = 0, e3 = 0, e5 = 0, e7 = 0;
    uint64_t nn = (uint64_t)n;

    /* exponent of 2 */
    for (uint64_t p = 2; p <= nn; p <<= 1)
        e2 += nn / p;

    /* exponent of 3 */
    for (uint64_t p = 3; p <= nn; p *= 3)
        e3 += nn / p;

    /* exponent of 5 */
    for (uint64_t p = 5; p <= nn; p *= 5)
        e5 += nn / p;

    /* exponent of 7 */
    for (uint64_t p = 7; p <= nn; p *= 7)
        e7 += nn / p;

    /* ----- Greedy construction of digits ----- */
    /* count[ d ] holds how many times digit d will appear (d=0..9) */
    uint32_t count[10] = {0};

    while (e3 >= 2) {            /* 9 uses 3^2 */
        count[9]++;
        e3 -= 2;
    }
    while (e2 >= 3) {            /* 8 uses 2^3 */
        count[8]++;
        e2 -= 3;
    }
    while (e7 >= 1) {            /* 7 uses 7 */
        count[7]++;
        e7 -= 1;
    }
    while (e2 >= 1 && e3 >= 1) { /* 6 uses 2*3 */
        count[6]++;
        e2 -= 1;
        e3 -= 1;
    }
    while (e5 >= 1) {            /* 5 uses 5 */
        count[5]++;
        e5 -= 1;
    }
    while (e2 >= 2) {            /* 4 uses 2^2 */
        count[4]++;
        e2 -= 2;
    }
    while (e3 >= 1) {            /* 3 uses 3 */
        count[3]++;
        e3 -= 1;
    }
    while (e2 >= 1) {            /* 2 uses 2 */
        count[2]++;
        e2 -= 1;
    }

    /* If no digits were added, the product is 1 → answer is 1. */
    if (count[2] + count[3] + count[4] + count[5] + count[6] +
        count[7] + count[8] + count[9] == 0)
        return (seq_t)1;

    /* ----- Assemble the minimal number ---- */
    seq_t result = 0;
    for (int d = 2; d <= 9; ++d)
        for (uint32_t k = 0; k < count[d]; ++k)
            result = result * (seq_t)10 + (seq_t)d;

    return result;
}
