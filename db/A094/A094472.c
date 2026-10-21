/*
 * A094472  a(n) = n*tau(n) - sigma(n) - phi(n), where tau(n) is the number of
 *          divisors of n.
 * https://oeis.org/A094472
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = n*tau(n) - sigma(n) - phi(n)
 * Range:     i8 n <= 12, i16 n <= 632, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   factor_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 *    tau(n)    : number of divisors of n
 *    sigma(n)  : sum of divisors of n
 *    phi(n)    : Euler's totient function
 * The function uses factor_uint64 from the utils directory to factor n.
 * All arithmetic is performed in the seq_t type (signed, possibly < 128‑bit).
 */
#include <stdint.h>
#include "../utils/factor_uint64.h"

seq_t a(idx_t n) {
    /* Factorisation of n (returns primes and exponents). */
    uint64_t primes[20];
    int exp[20];
    int cnt;
    factor_uint64((uint64_t)n, primes, exp, &cnt);

    seq_t tau  = 1;        /* product of (ei+1) */
    seq_t phi  = 1;        /* product of (p-1)*p^(e-1) */
    seq_t sigma = 1;        /* product of (p^(e+1)-1)/(p-1) */

    for (int i = 0; i < cnt; ++i) {
        /* tau */
        tau *= (seq_t)(exp[i] + 1);

        /* phi: (p-1)*p^(e-1) */
        seq_t p_pow = 1;
        for (int j = 0; j < exp[i] - 1; ++j) {
            p_pow *= (seq_t)primes[i];
        }
        phi *= (seq_t)(primes[i] - 1) * p_pow;

        /* sigma: (p^(e+1)-1)/(p-1) */
        seq_t p_pow_e1 = 1;
        for (int j = 0; j < exp[i] + 1; ++j) {
            p_pow_e1 *= (seq_t)primes[i];
        }
        seq_t term = (p_pow_e1 - 1) / (seq_t)(primes[i] - 1);
        sigma *= term;
    }

    /* Handle the case n == 1 (cnt == 0). tau=1, phi=1, sigma=1 by default. */

    seq_t result = (seq_t)n * tau - sigma - phi;
    return result;
}
