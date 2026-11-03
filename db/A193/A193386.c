/*
 * A193386  Number of even divisors of phi(n).
 * https://oeis.org/A193386
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of even divisors of φ(n)
 * Range:     u8 n <= 303, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 121 terms through n = 9795 (2026-11-01)
 * Helpers:   factor_u64, totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/totient.h"

seq_t a(idx_t n)
{
    /* Compute Euler's totient φ(n) */
    seq_t tot = totient(n);

    /* φ(n) fits in uint64_t because n is uint64_t */
    uint64_t phi_val = (uint64_t)tot;

    /* Factor φ(n) into prime bases and exponents */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(phi_val, primes, exps, &cnt);

    /* τ(φ(n)) and τ_odd(φ(n)) */
    seq_t tau = 1;
    seq_t tau_odd = 1;

    for (size_t i = 0; i < cnt; ++i) {
        seq_t e = (seq_t)exps[i];
        seq_t term = e + 1;

        tau *= term;

        if (primes[i] != 2) {
            tau_odd *= term;
        }
    }

    /* Even divisors = total divisors – odd divisors */
    return tau - tau_odd;
}
