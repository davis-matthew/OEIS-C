/*
 * A124655  a(n) = n-th integer from among those positive integers k where
 *          every positive integers <= k and coprime to k is also coprime to
 *          n.
 * https://oeis.org/A124655
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the n‑th integer k such that every integer <= k that is coprime to k
   is also coprime to n.  For each fixed n we search for the n‑th such k. */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* collect unique prime factors of n */
    uint64_t primes[64];
    size_t pc = 0;
    uint64_t t = n;
    for (uint64_t p = 2; p * p <= t; ++p) {
        if (t % p == 0) {
            primes[pc++] = p;
            while (t % p == 0) t /= p;
        }
    }
    if (t > 1) primes[pc++] = t;

    uint64_t count = 0;
    for (uint64_t k = 1; ; ++k) {
        bool ok = true;
        for (size_t i = 0; i < pc; ++i) {
            uint64_t p = primes[i];
            if (p <= k) {
                if (k % p != 0) {
                    ok = false;
                    break;
                }
            }
        }
        if (ok) {
            ++count;
            if (count == n) {
                return (seq_t)k;
            }
        }
    }
}
