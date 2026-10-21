/*
 * A050469  a(n) = Sum_{ d divides n, n/d=1 mod 4} d - Sum_{ d divides n,
 *          n/d=3 mod 4} d.
 * https://oeis.org/A050469
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) is multiplicative. For a prime power p^e the value is
 *            a(2^e) = 2^e, a(p^e) for p≡1 (mod4) = (p^(e+1)-1)/(p-1), a(p^e)
 *            for p≡3 (mod4) = (p^(e+1)+(-1)^e)/(p+1)
 * Range:     u8 n <= 16, u16 n <= 2390, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor.h"

seq_t a(idx_t n)
{
    uint64_t pr[64];
    uint64_t exp[64];
    size_t cnt;
    factor(n, pr, exp, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p_u = pr[i];
        uint64_t e   = exp[i];
        seq_t p      = (seq_t)p_u;
        seq_t term;

        if (p_u == 2) {                     /* a(2^e) = 2^e */
            seq_t pow = 1;
            for (uint64_t k = 0; k < e; ++k) pow *= 2;
            term = pow;
        } else if ((p_u & 3ULL) == 1ULL) {   /* p ≡ 1 (mod 4) */
            seq_t pow = 1;
            for (uint64_t k = 0; k < e + 1; ++k) pow *= p;
            term = (pow - 1) / (p - 1);
        } else {                             /* p ≡ 3 (mod 4) */
            seq_t pow = 1;
            for (uint64_t k = 0; k < e + 1; ++k) pow *= p;
            if ((e & 1ULL) == 0)             /* even e: (-1)^e = 1 */
                term = (pow + 1) / (p + 1);
            else                             /* odd e:  (-1)^e = -1 */
                term = (pow - 1) / (p + 1);
        }

        result *= term;
    }
    return result;
}
