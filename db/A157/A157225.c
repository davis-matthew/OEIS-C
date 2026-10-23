/*
 * A157225  Number of ways to write the n-th positive odd integer in the form
 *          p+2^x+7*2^y with p a prime congruent to 5 mod 6 and x,y positive
 *          integers.
 * https://oeis.org/A157225
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nice, nonn
 * Formula:   a(n) = number of representations of 2n‑1 as p + 2^x + 7·2^y with
 *            p a prime ≡5 (mod 6) and x, y ≥1. The routine enumerates all
 *            admissible pairs (x, y) and checks the resulting p for primality
 *            and congruence
 * Range:     u8 n <= 1983, u16 n <= 8925, u32 n <= 8925, u64 n <= 8925,
 *            u128 n <= 8925, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* Target odd integer 2n‑1 */
    seq_t T = (seq_t)2 * n - 1;

    /* Too small to contain a prime ≥5 and two powers of two */
    if (T < 5) return (seq_t)0;

    seq_t count = 0;
    uint64_t limit = (uint64_t)T - 5;          /* maximum possible p–5 */

    /* iterate over 2^x, x ≥ 1 */
    for (uint64_t pow2x = 2; pow2x <= limit; pow2x <<= 1) {
        uint64_t rem_after_x = limit - pow2x;  /* remaining := T – 5 – 2^x */

        /* iterate over 2^y, y ≥ 1 */
        for (uint64_t pow2y = 2; 7 * pow2y <= rem_after_x; pow2y <<= 1) {
            seq_t p = T - (seq_t)pow2x - (seq_t)7 * (seq_t)pow2y;

            if (p >= 5 && (p % 6 == 5) && is_prime((idx_t)p))
                ++count;
        }
    }
    return count;
}
