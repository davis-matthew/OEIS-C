/*
 * A003535  Divisors of 2^27 - 1.
 * https://oeis.org/A003535
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = n-th smallest divisor of 2^27 - 1 = 7 * 73 * 262657;
 *            found by trial division up to sqrt(2^27 - 1)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    const uint64_t N = (((uint64_t)1) << 27) - 1;
    uint64_t lo[64], hi[64];
    int nlo = 0, nhi = 0;
    uint64_t i;

    for (i = 1; i * i <= N; i++) {
        if (N % i == 0) {
            lo[nlo++] = i;
            if (i != N / i)
                hi[nhi++] = N / i;
        }
    }

    /* lo is ascending, hi is descending */
    if (n < 1)
        return (seq_t)0;
    if (n <= (idx_t)nlo)
        return (seq_t)lo[n - 1];
    if (n <= (idx_t)(nlo + nhi))
        return (seq_t)hi[nhi - 1 - (int)(n - 1 - (idx_t)nlo)];
    return (seq_t)0;
}
