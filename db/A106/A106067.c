/*
 * A106067  Primes p such that 3*p + 2 and 2*p + 3 are primes.
 * https://oeis.org/A106067
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   isprime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A106067: primes p such that 3*p+2 and 2*p+3 are also primes.
   Computed by incremental search using the deterministic primality test
   provided by the build (isprime_big). */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/isprime_big.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)5;
    if (n == 2) return (seq_t)7;

    idx_t count = 2;            /* we already counted 5 and 7 */

    /* Wheel: numbers coprime to 2,3,5.  (mod 30) */
    static const int wheel[8] = {1, 7, 11, 13, 17, 19, 23, 29};
    uint64_t base = 0;
    int idx = 0;

    while (1) {
        /* Build next candidate */
        uint64_t cand = base + wheel[idx];
        idx++;
        if (idx == 8) { idx = 0; base += 30; }

        /* Skip values < 5 (5 and 7 already handled) */
        if (cand < 5) continue;
        if (cand == 5 || cand == 7) continue;

        /* Test primality of cand, 3*cand+2, and 2*cand+3 */
        if (!isprime_big((seq_t)cand)) continue;

        seq_t y = (seq_t)cand * 3 + 2;
        seq_t z = (seq_t)cand * 2 + 3;

        if (isprime_big(y) && isprime_big(z)) {
            count++;
            if (count == n) return (seq_t)cand;
        }
    }
}
