/*
 * A004794  Least positive integer k such that the fractional part of
 *          k*sqrt(5) has its n initial partial quotients all equal to 1.
 * https://oeis.org/A004794
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (Fib(12*floor(n/6) + S_(n mod 6)) + 1)/2, S = (2, 5, 7,
 *            7, 10, 11); computed as Fib(k-2) + ceil(Fib(k-3)/2), since
 *            Fib(k) = 2*Fib(k-2) + Fib(k-3)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 23, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   fib_pair
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/fib_pair.h"

seq_t a(idx_t n) {
    static const int S[] = {2, 5, 7, 7, 10, 11};
    uint64_t k = 12 * (uint64_t)(n / 6) + (uint64_t)S[n % 6];
    seq_t f3, f2;
    /* k >= 5 for n >= 1 */
    fib_pair(k - 3, &f3, &f2);       /* f3 = F(k-3), f2 = F(k-2) */
    return f2 + f3 / 2 + (f3 & 1);
}
