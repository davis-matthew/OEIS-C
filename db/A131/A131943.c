/*
 * A131943  Expansion of b(q) * b(q^2) in powers of q where b() is a cubic AGM
 *          theta function.
 * https://oeis.org/A131943
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -3 * b(n), where b() is multiplicative with b(2^e) = 1,
 *            b(3^e) = 4 - 3^(e+1), b(p^e) = (p^(e+1) - 1) / (p - 1) for p > 3
 * Range:     i8 n <= 12, i16 n <= 53, i32 n <= 53, i64 n <= 53, i128 n <= 53,
 *            big any n
 * Helpers:   factor_u64, pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_int.h"
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* Handle the offset carefully. */
    if (n == 0) return (seq_t)1;

    /* Remove all powers of 2, because b(2^e) = 1. */
    idx_t odd = n;
    while ((odd & 1ULL) == 0 && odd > 1)
        odd >>= 1;

    /* If only 2's were present, odd == 1.  Then b(n) = 1. */
    seq_t b_val = (seq_t)1;

    if (odd > 1) {
        /* Factor the odd part of n. */
        uint64_t primes[64];
        uint64_t exps[64];
        size_t cnt = 0;

        factor_u64(odd, primes, exps, &cnt);

        for (size_t i = 0; i < cnt; ++i) {
            uint64_t p = primes[i];
            idx_t e = (idx_t)exps[i];
            seq_t term;

            if (p == 3ULL) {
                /* b(3^e) = 4 - 3^(e+1) */
                term = (seq_t)4 - pow_int((seq_t)3, e + 1);
            } else {
                /* b(p^e) = (p^(e+1) - 1) / (p - 1) */
                seq_t numerator = pow_int((seq_t)p, e + 1) - (seq_t)1;
                seq_t denominator = (seq_t)(p - 1);
                term = numerator / denominator; /* Exact division. */
            }

            b_val *= term;
        }
    }

    /* Return a(n) = -3 * b(n). */
    return - (seq_t)3 * b_val;
}
