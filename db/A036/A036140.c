/*
 * A036140  a(n) = 2^n mod 107.
 * https://oeis.org/A036140
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n mod 107
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Periodic recurrence: a(n+106) = a(n). */
/* This implementation uses binary exponentiation modulo 107. */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* we restrict intermediate multiplications to 64‑bit to avoid overflow */
    uint64_t mod = 107ULL;
    seq_t res = (seq_t)1;
    seq_t base = (seq_t)(2ULL % mod);
    idx_t exp = n;

    while (exp > 0) {
        if (exp & 1ULL) {
            uint64_t prod = (uint64_t)res * (uint64_t)base;
            res = (seq_t)(prod % mod);
        }
        uint64_t prod = (uint64_t)base * (uint64_t)base;
        base = (seq_t)(prod % mod);
        exp >>= 1;
    }
    return res;
}
