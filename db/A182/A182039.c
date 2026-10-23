/*
 * A182039  Order of the group O(2,Z_n); number of orthogonal 2 X 2 matrices
 *          over the ring Z/nZ.
 * https://oeis.org/A182039
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = A060968(n) * A060594(n) Multiplicative: a(p^e) =
 *            2*(p-1)*p^(e-1) if p≡1 (mod 4), a(p^e) = 2*(p+1)*p^(e-1) if p≡3
 *            (mod 4), special for p=2: a(2)=2, a(4)=16, a(2^e)=2^(e+3) for
 *            e≥3. Convolution of the above over prime factors
 * Range:     u8 n <= 23, u16 n <= 839, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/pow_seq.h"

seq_t a(idx_t n) {
    if (n == 1)
        return (seq_t)1;

    seq_t result = (seq_t)1;

    /* factorize n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    for (size_t i = 0; i < cnt; i++) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];

        if (p == 2) {
            if (e == 1) {
                result *= (seq_t)2;
            } else if (e == 2) {
                result *= (seq_t)16;
            } else {
                /* 2^(e+3) */
                seq_t powv = pow_seq((seq_t)2, (int)(e + 3));
                result *= powv;
            }
        } else {
            /* 2 * (p ± 1) */
            seq_t mult = 2 *
                         ((p % 4 == 1) ? (p - 1) : (p + 1));
            /* p^(e-1) */
            seq_t powv = pow_seq((seq_t)p, (int)(e - 1));
            result *= mult;
            result *= powv;
        }
    }

    return result;
}
