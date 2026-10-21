/*
 * A078694  Numbers n such that floor(2*Pi*n) is prime.
 * https://oeis.org/A078694
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers n such that floor(2*Pi*n) is prime, offset 1 */
/* This implementation scans integers in order and keeps a counter.   */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* 2*Pi scaled to 10^18 (truncated).                         */
    const uint64_t SCALE = 1000000000000000000ULL;
    const uint64_t TWO_PI_SCALED = 6283185307179586476ULL;  /* floor(2π*10^18) */

    idx_t count = 0;
    seq_t candidate = 1;                       /* first positive integer */

    while (1) {
        /* Use 128‑bit arithmetic for the product */
        __uint128_t prod = (__uint128_t)TWO_PI_SCALED * (uint64_t)candidate;
        seq_t circ = (seq_t)(prod / SCALE);   /* floor(2π*candidate) */

        /* Test primality of the integer part */
        if (is_prime((idx_t)circ))
        {
            count++;
            if (count == n)
                return candidate;
        }
        candidate++;
    }
}
