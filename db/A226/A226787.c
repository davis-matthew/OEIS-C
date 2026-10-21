/*
 * A226787  If n=0 (mod 3) then a(n)=0, otherwise a(n)=9^(-1) in Z/nZ*.
 * https://oeis.org/A226787
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 0 if n is divisible by 3 (or n = 1), otherwise a(n) =
 *            9^(-1) mod n, the multiplicative inverse of 9 modulo n
 * Range:     u8 n <= 288, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mod_pow.h"   /* mod_pow(seq_t base, uint64_t exp, seq_t mod) */

seq_t a(idx_t n) {
    /* Off‑shelf rule: anything divisible by 3 has no inverse of 9. */
    if (n == 1 || (n % 3) == 0)
        return 0;

    /* Compute Euler's totient φ(n) by trial division (n ≤ 10⁴ in typical tests). */
    idx_t m = n;
    idx_t phi = m;
    for (idx_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            phi = phi / p * (p - 1);
            while (m % p == 0)
                m /= p;
        }
    }
    if (m > 1)          /* n has a remaining prime factor > sqrt(n) */
        phi = phi / m * (m - 1);

    /* φ(n) ≥ 1; φ(n)-1 fits in idx_t for all realistic n. */
    idx_t exp = phi - 1;
    seq_t mod = (seq_t)n;

    /* Apply Euler's theorem: 9^(φ(n)-1) ≡ 9^(-1) (mod n). */
    seq_t base = (seq_t)9;
    return mod_pow(base, exp, mod);
}
