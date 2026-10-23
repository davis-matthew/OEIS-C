/*
 * A168669  Numbers n such that sqrt(36*n+49) is prime.
 * https://oeis.org/A168669
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 81, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = (p^2 - 49) / 36, where p is the nth prime with p^2 ≡ 13 (mod 36) */
#include "../utils/next_prime.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    uint64_t p = 7;                 /* first prime satisfying the condition */
    while (1) {
        uint64_t r = p % 36u;
        if ((r * r) % 36u == 13u) {
            seq_t val = ((seq_t)p * (seq_t)p - (seq_t)49) / (seq_t)36;
            ++count;
            if (count == n) return val;
        }
        p = next_prime(p);
    }
}
