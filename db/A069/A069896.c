/*
 * A069896  GCD of consecutive values of Chowla's function.
 * https://oeis.org/A069896
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = gcd(sigma(n+1)-(n+1)-1, sigma(n)-n-1)
 * Range:     u8 n <= 166, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sigma_diff
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_diff.h"

seq_t a(idx_t n) {
    if (n <= 1) return 0;                    /* by definition, a(1)=0          */

    unsigned long long diff1 = sigma_diff((unsigned long long)(n + 1));
    unsigned long long diff2 = sigma_diff((unsigned long long)n);

    /* Euclidean algorithm on unsigned long long */
    while (diff2 != 0) {
        unsigned long long r = diff1 % diff2;
        diff1 = diff2;
        diff2 = r;
    }
    return (seq_t)diff1;                     /* cast back to seq_t              */
}
