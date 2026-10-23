/*
 * A138809  Expansion of 8 * eta(q)^7 / eta(q^7) + 49 * (eta(q) * eta(q^7))^3
 *          in powers of q.
 * https://oeis.org/A138809
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -7 * b(n), where b(n) is multiplicative; b(7^e) = 1,
 *            ((p^2)^(e+1)-1)/(p^2-1) if p ≡ 1,2,4 (mod 7),
 *            (-(-p^2)^(e+1)+1)/(p^2+1) if p ≡ 3,5,6 (mod 7)
 * Range:     i8 n <= 3, i16 n <= 16, i32 n <= 2845, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   _smallest_prime_factor, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/_smallest_prime_factor.h"
#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    /* a(0) is special – the multiplicative definition does not
       cover the empty factorisation.  */
    if (n == 0)
        return (seq_t)8;

    uint64_t nn = (uint64_t)n;
    seq_t prod = (seq_t)1;

    while (nn > 1) {
        uint64_t p = _smallest_prime_factor(nn);
        uint64_t exp = 0;
        while (nn % p == 0) {
            nn /= p;
            exp++;
        }

        /* p = 7 contributes a factor 1, so skip. */
        if (p == 7)
            continue;

        seq_t p2 = (seq_t)p * (seq_t)p;   /* p^2 */
        seq_t contrib;

        uint64_t pm = p % 7;
        if (pm == 1 || pm == 2 || pm == 4) {
            /* ((p^2)^(e+1)-1)/(p^2-1) */
            seq_t num = pow_seq(p2, (int)(exp + 1)) - 1;
            seq_t den = p2 - 1;
            contrib = num / den;
        } else { /* pm == 3,5,6 */
            /* (-(-p^2)^(e+1)+1)/(p^2+1) */
            seq_t base = -(seq_t)p2;
            seq_t num = -pow_seq(base, (int)(exp + 1)) + 1;
            seq_t den = p2 + 1;
            contrib = num / den;
        }

        prod *= contrib;
    }

    return -(seq_t)7 * prod;
}
