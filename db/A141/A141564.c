/*
 * A141564  Subtract 1 from all bases and exponents which are greater than 1
 *          in the prime number decomposition of n.
 * https://oeis.org/A141564
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ∏_p (p-1)^(e-1) for n=p^e, e≥2; a(p) = p-1; a(1) = 0
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   factor_u64, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Multiplicative function defined by prime‑power values:
   a(p)   = p-1          (e = 1)
   a(p^e) = (p-1)^(e-1)  (e ≥ 2)
   a(n)   = ∏ a(p^e) for the prime powers of n. */

#include "../utils/factor_u64.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* Special case: the sequence has a(1) = 0 */
    if (n == 1)
        return 0;

    uint64_t x = (uint64_t)n;
    seq_t result = 1;

    /* temporary arrays large enough for all prime factors of a 64‑bit integer */
    static uint64_t primes[64];
    static uint64_t exps[64];
    size_t cnt = 0;

    /* factorize n into primes and exponents */
    factor_u64(x, primes, exps, &cnt);

    /* multiply the contribution of each prime power */
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        seq_t term;
        if (e == 1) {
            term = (seq_t)(p - 1);          /* a(p)          */
        } else {
            term = pow_uint(p - 1, e - 1);   /* a(p^e), e ≥ 2 */
        }
        result = result * term;
    }

    return result;
}
