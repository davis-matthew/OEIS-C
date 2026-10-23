/*
 * A122854  Expansion of phi(q)^2*psi(q)^4 in powers of q where phi(),psi()
 *          are Ramanujan theta functions.
 * https://oeis.org/A122854
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = b(2n+1) where b is multiplicative: * b(2^e) = 0^e *
 *            b(p^e) = ((p^2)^(e+1)-1)/(p^2-1) if p ≡ 1 (mod 4) *
 *            ((p^2)^(e+1)-(-1)^(e+1))/(p^2+1) if p ≡ 3 (mod 4)
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 509, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   factor_u64, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Dependencies: factor_u64, seq_pow. */

#include "../utils/factor_u64.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n) {
    /* m = 2n + 1 , always odd */
    uint64_t m = (uint64_t)(2 * (uint64_t)n + 1);
    if (m == 1) return (seq_t)1;   /* a(0) = 1 */

    /* factorize m */
    size_t cnt = 0;
    uint64_t primes[64];
    uint64_t exps[64];
    factor_u64(m, primes, exps, &cnt);

    seq_t result = (seq_t)1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        /* skip factor 2 (should not occur); treat as zero factor if needed */
        if (p == 2) {
            if (e > 0) return (seq_t)0;  /* b(2^e)=0^e -> 0 for e>0 */
            continue;
        }

        seq_t p2 = (seq_t)p * (seq_t)p;
        seq_t denom;
        seq_t num;
        seq_t pow_val = seq_pow(p2, (idx_t)(e + 1));

        if (p % 4 == 1) {
            denom = p2 - 1;
            num   = pow_val - 1;
        } else { /* p % 4 == 3 */
            denom = p2 + 1;
            if (((e + 1) & 1) == 1)   /* odd exponent+1 => (-1)^(e+1) = -1 */
                num = pow_val + 1;    /* subtract (-1) => add 1 */
            else
                num = pow_val - 1;    /* subtract (+1) => subtract 1 */
        }
        result *= (num / denom);
    }

    return result;
}
