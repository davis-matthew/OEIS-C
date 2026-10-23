/*
 * A193453  Number of odd divisors of phi(n).
 * https://oeis.org/A193453
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1704, u16 n <= 9837, u32 n <= 9837, u64 n <= 9837,
 *            u128 n <= 9837, big any n
 * Helpers:   factor_u64, odd_part, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of odd divisors of Euler's totient φ(n).  The function
   returns d_odd(φ(n)) = d(odd_part(φ(n))), where d(m) is the
   number of divisors of m.  It uses the helpers `phi`, `odd_part`
   and `factor_u64` provided in the usual utility headers. */

#include "../utils/phi.h"
#include "../utils/odd_part.h"
#include "../utils/factor_u64.h"

seq_t a(idx_t n) {
    /* Compute φ(n) */
    seq_t phi_n = phi(n);

    /* Extract the odd part of φ(n) */
    seq_t odd = odd_part(phi_n);

    /* φ(n) ≤ 2^64–1, so the odd part also fits in 64 bits */
    uint64_t m = (uint64_t)odd;

    /* Factor the odd part and count its divisors */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    factor_u64(m, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        result *= (seq_t)(exps[i] + 1);   /* multiply by (exponent + 1) */
    }

    return result;
}
