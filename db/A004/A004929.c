/*
 * A004929  a(n) = floor(n*phi^14), where phi is the golden ratio, A001622.
 * https://oeis.org/A004929
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n*phi^14), where phi^14 = 843 - psi^14, psi = (1 -
 *            sqrt(5))/2
 * Range:     u8 n <= 0, u16 n <= 77, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n) {
    const long double sqrt5 = sqrtl(5.0L);
    const long double psi = (1.0L - sqrt5) / 2.0L;
    const long double psi14 = powl(psi, 14.0L);
    const long double phi14 = 843.0L - psi14;
    return (seq_t)(long long)((long double)n * phi14);
}
