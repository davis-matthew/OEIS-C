/*
 * A011773  Variant of Carmichael's lambda function: a(p1^e1*...*pN^eN) =
 *          lcm((p1-1)*p1^(e1-1), ..., (pN-1)*pN^(eN-1)).
 * https://oeis.org/A011773
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = lcm_{p^e∥n} (p-1)·p^(e-1) , where a(1)=1
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/gcd.h"

seq_t a(idx_t n) {
    if (n == 1)
        return (seq_t)1;

    uint64_t num = (uint64_t)n;
    seq_t result = 1;

    /* trial division factorisation of n */
    for (uint64_t p = 2; p <= num / p; ++p) {
        if (num % p == 0) {
            unsigned e = 0;
            while (num % p == 0) {
                num /= p;
                ++e;
            }

            /* Compute (p-1) · p^(e-1) */
            seq_t term = (seq_t)(p - 1);
            if (e > 1) {
                seq_t pow = 1;
                for (unsigned i = 1; i < e; ++i)
                    pow *= (seq_t)p;
                term *= pow;
            }

            /* Update result = lcm(result, term) */
            seq_t g = gcd(result, term);
            seq_t tmp = result / g;
            result = tmp * term;
        }
    }

    /* Remaining prime factor, if any */
    if (num > 1) {
        seq_t term = (seq_t)(num - 1);
        seq_t g = gcd(result, term);
        seq_t tmp = result / g;
        result = tmp * term;
    }

    return result;
}
