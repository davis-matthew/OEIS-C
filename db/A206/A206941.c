/*
 * A206941  a(n) = phi(lambda(n)), where phi = A000010, lambda = A002322.
 * https://oeis.org/A206941
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = phi(lambda(n))
 * Range:     u8 n <= 556, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   calc_phi, factor_u64, gcd, pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  lambda(n) is the Carmichael function (A002322)  */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Utility headers – adjust names if needed */
#include "../utils/factor_u64.h"
#include "../utils/pow_int.h"
#include "../utils/gcd.h"
#include "../utils/calc_phi.h"

static seq_t lcm_seq(seq_t a, seq_t b)
{
    seq_t g = gcd(a, b);
    return (a / g) * b;
}

seq_t a(idx_t n)
{
    if (n < 1)       /* undefined for n < 1 */
        return 0;

    if (n == 1)      /* Carmichael of 1 is 1, phi(1)=1 */
        return 1;

    /* Factor n (up to uint64_t for this implementation) */
    uint64_t primes[64], exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t lambda = 1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        seq_t lpow;

        if (p == 2) {
            if (e == 1)          lpow = 1;
            else if (e == 2)     lpow = 2;
            else                 lpow = pow_int(2U, e - 2);
        } else {
            seq_t p_pow = pow_int((seq_t)p, (idx_t)(e - 1));
            lpow = p_pow * ((seq_t)(p - 1));
        }

        lambda = lcm_seq(lambda, lpow);
    }

    return calc_phi(lambda);
}
