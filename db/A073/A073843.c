/*
 * A073843  a(1) = 1; for n > 1 a(n) = smallest number of the form n^r (with r
 *          rational != 1) not included earlier.
 * https://oeis.org/A073843
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^((b - (-1)^b)/b) where b is the gcd of the * exponents
 *            in the prime factorization of n. * For a given n >= 1, the
 *            function returns the corresponding term * of OEIS sequence
 *            A073843
 * Range:     u8 n <= 16, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   big_pow, factor_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/factor_u64.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Handle trivial case. */
    if (n == (idx_t)1) {
        return (seq_t)1;
    }

    /* Factorise n.  At most 64 distinct prime factors for n < 2^64. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t  cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* Compute b = gcd of the exponents. */
    seq_t b = (seq_t)0;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t e = (seq_t)exps[i];
        b = (i == 0) ? e : gcd(b, e);
    }

    /* Compute the integer b‑th root of n:
     *   root = Π p_i^(e_i / b)
     */
    seq_t root = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t exp_div = exps[i] / (uint64_t)b;   /* exact division */
        seq_t   term = big_pow((seq_t)primes[i], (seq_t)exp_div);
        root *= term;
    }

    /* Assemble final result.  Odd b → multiply; even b → divide. */
    seq_t n_seq = (seq_t)n;
    seq_t result;
    if ((b & (seq_t)1) == (seq_t)1) {   /* b odd */
        result = n_seq * root;
    } else {                            /* b even */
        result = n_seq / root;
    }
    return result;
}
