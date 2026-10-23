/*
 * A170817  a(n) = product of distinct primes of form 4k+1 that divide n.
 * https://oeis.org/A170817
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = product of distinct primes of form 4k+1 that divide n
 * Range:     u8 n <= 256, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n < 1) return 0; /* undefined for n < 1 */

    uint64_t nn = (uint64_t)n;   /* work on a 64‑bit value for factorisation */
    seq_t result = 1;

    for (uint64_t d = 2; d * d <= nn; ++d) {
        if (nn % d == 0) {
            if ((d & 3) == 1)                /* d ≡ 1 (mod 4) */
                result *= (seq_t)d;           /* multiply only once per prime */
            while (nn % d == 0) nn /= d;      /* remove all powers of d */
        }
    }

    /* Any remaining prime factor greater than sqrt(nn) */
    if (nn > 1 && (nn & 3) == 1)
        result *= (seq_t)nn;

    return result;
}
