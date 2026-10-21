/*
 * A060640  If n = Product p_i^e_i then a(n) = Product (1 + 2*p_i + 3*p_i^2 +
 *          ... + (e_i+1)*p_i^e_i).
 * https://oeis.org/A060640
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy, mult
 * Formula:   a(n) = Π_{p^e‖n} Σ_{k=0}^e (k+1)·p^k Compute by trial‑division
 *            factorisation (fast enough for n up to ~10⁶). For larger n the
 *            routine remains correct, only slower
 * Range:     u8 n <= 23, u16 n <= 1007, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1)                /* offset starts at 1; behaviour for 0 or negatives is irrelevant */
        return 0;

    seq_t result = 1;
    uint64_t m = (uint64_t)n;
    uint64_t p, e;

    /* factorise m by trial division */
    for (p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }

            /* compute Σ_{k=0}^e (k+1)·p^k */
            seq_t sum   = 0;
            seq_t power = 1;
            for (uint64_t k = 0; k <= e; ++k) {
                sum   += (seq_t)(k + 1) * power;
                power *= (seq_t)p;
            }
            result *= sum;
        }
    }

    /* if any prime factor > sqrt(original n) remains */
    if (m > 1) {
        /* m is prime, exponent e = 1 */
        sum:
        seq_t sum   = 0;
        seq_t power = 1;
        e = 1;
        for (uint64_t k = 0; k <= e; ++k) {
            sum   += (seq_t)(k + 1) * power;
            power *= (seq_t)m;
        }
        result *= sum;
    }

    return result;
}
