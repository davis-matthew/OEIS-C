/*
 * A119733  Offsets of the terms of the nodes of the reverse Collatz function.
 * https://oeis.org/A119733
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0; if n is even: a(n) = 2 * a(n/2); * if n is odd: a(n) =
 *            2 * a(n/2) + 3^popcount(n/2)
 * Range:     u8 n <= 58, u16 n <= 2030, u32 n <= 8500, u64 n <= 8500,
 *            u128 n <= 8500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recursively defined with a tiny helper for popcount and 3^k */

#include <stdint.h>

/* popcount for 64‑bit values – works with GCC/Clang and is portable to C++17 */
static inline unsigned int popcnt64(uint64_t x)
{
    return __builtin_popcountll(x);
}

/* 3^exp computed iteratively (exp <= 63 for our range) */
static inline seq_t pow3_uint(unsigned int exp)
{
    seq_t r = 1;
    while (exp--)
        r *= 3;
    return r;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    idx_t half = n >> 1;          /* floor(n/2) */
    seq_t res = 2 * a(half);      /* 2 * a(half) */

    if (n & 1)                    /* odd case */
        res += pow3_uint(popcnt64(half));

    return res;
}
