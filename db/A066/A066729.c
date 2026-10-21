/*
 * A066729  a(n) = Product_{d|n, d<n} d if n is composite, n otherwise.
 * https://oeis.org/A066729
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = n if n is prime; otherwise a(n) = n^{(d(n)-2)/2}, where
 *            d(n) is the number of divisors of n. This equals the product of
 *            all proper divisors of a composite number
 * Range:     u8 n <= 17, u16 n <= 35, u32 n <= 89, u64 n <= 239,
 *            u128 n <= 719, big any n
 * Helpers:   factor_u64, floor_sqrt, is_prime, seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/is_prime.h"
#include "../utils/factor_u64.h"
#include "../utils/seq_pow.h"
#include "../utils/floor_sqrt.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    /* a(1) = 1 by definition. */
    if (n <= 1)
        return (seq_t)1;

    /* For primes the definition gives a(n) = n. */
    if (is_prime(n))
        return (seq_t)n;

    /* Factor n into its prime powers. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* Compute the divisor function d(n) = Π (ei + 1). */
    idx_t d = 1;
    for (size_t i = 0; i < cnt; ++i)
        d *= (idx_t)(exps[i] + 1);

    /* a(n) = n^{(d-2)/2}.  When d is odd the exponent is a
       half‑integer; this happens only for perfect squares. */
    if (d & 1) {             /* odd d → perfect square */
        idx_t k = (d - 3) / 2;          /* integer part of exponent */
        seq_t sqrt_n = floor_sqrt((seq_t)n);
        seq_t pow_n  = seq_pow((seq_t)n, k);
        return seq_mul(sqrt_n, pow_n);
    } else {                 /* even d → integer exponent */
        idx_t k = (d - 2) / 2;
        return seq_pow((seq_t)n, k);
    }
}
