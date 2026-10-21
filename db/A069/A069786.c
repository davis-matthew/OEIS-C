/*
 * A069786  Smallest k>n such that C(n+3,n)=A000332(n) divides A000332(k).
 * https://oeis.org/A069786
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = smallest k>n such that C(n+3,4) divides C(k+3,4) * where
 *            C(x,4) = x(x-1)(x-2)(x-3)/24 (i.e. (x)(x-1)(x-2)(x-3)/4!). *
 *            This implements exactly the definition of OEIS A069786
 * Range:     u8 n <= 14, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* divisor d = C(n+3,4) = (n+3)(n+2)(n+1)n / 24  */
    unsigned long long d = ((unsigned long long)(n+3) *
                            (unsigned long long)(n+2) *
                            (unsigned long long)(n+1) *
                            (unsigned long long)(n)) / 24ULL;

    for (idx_t k = n + 1; ; ++k) {
        unsigned long long f = ((unsigned long long)(k+3) *
                                (unsigned long long)(k+2) *
                                (unsigned long long)(k+1) *
                                (unsigned long long)(k)) / 24ULL;
        if (f % d == 0) {
            return (seq_t)k;
        }
    }
}
