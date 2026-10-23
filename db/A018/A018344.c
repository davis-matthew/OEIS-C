/*
 * A018344  Divisors of 230.
 * https://oeis.org/A018344
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = product of the prime factors 2, 5, and 23 corresponding
 *            to the set bits of (n-1) – the divisors of 230 in increasing
 *            order (a(1)=1, a(2)=2, a(3)=5, a(4)=10, a(5)=23, a(6)=46,
 *            a(7)=115, a(8)=230)
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n < 1) return 0;          /* offset is 1 */
    if (n > 8) return 0;          /* sequence is finite */

    const uint64_t primes[3] = {2, 5, 23};
    idx_t mask = n - 1;           /* binary mask of selected primes */
    seq_t res = 1;

    for (int i = 0; i < 3; ++i)
        if (mask & (1ull << i))
            res *= (seq_t)primes[i];

    return res;
}
