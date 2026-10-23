/*
 * A124815  Expansion of q * psi(q)^2 * psi(-q^3)^2 * phi(-q^6) / phi(-q^2) in
 *          powers of q where phi(), psi() are Ramanujan theta functions.
 * https://oeis.org/A124815
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) is multiplicative: For p prime, if p < 5: a(p^e) = p^e if p
 *            ≡ 1 or 11 (mod 12): a(p^e) = (p^(e+1) – 1) / (p – 1) if p ≡ 5 or
 *            7 (mod 12): a(p^e) = (p^(e+1) + (–1)^e) / (p + 1) a(1) = 1
 * Range:     u8 n <= 16, u16 n <= 2850, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   factor_u64, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/pow_seq.h"
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n == 1) return 1;
    uint64_t primes[32];
    uint64_t exps[32];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        seq_t term;
        if (p < 5) {
            term = pow_seq((seq_t)p, (int)e);          /* p^e */
        } else if ((p % 12) == 1 || (p % 12) == 11) {
            seq_t numer = pow_seq((seq_t)p, (int)(e + 1));
            numer -= 1;
            term = numer / (seq_t)(p - 1);
        } else {   /* p ≡ 5 or 7 (mod 12) */
            seq_t numer = pow_seq((seq_t)p, (int)(e + 1));
            if (e % 2 == 0) numer += 1;
            else            numer -= 1;
            term = numer / (seq_t)(p + 1);
        }
        result *= term;
    }
    return result;
}
