/*
 * A208955  Expansion of phi(x) * phi(x^9) / chi(x^3)^2 in powers of x where
 *          phi(), chi() are Ramanujan theta functions.
 * https://oeis.org/A208955
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = b(4*n + 1) The multiplicative function b is defined by
 *            its prime power values b(p^e): b(2^e) = 0 (for any e ≥ 1) b(3^e)
 *            = 0 (for any e ≥ 1) if p ≠ 2,3 is an odd prime: if p ≡ 3 (mod
 *            4): b(p^e) = (1 + (-1)^e)/2 // 1 when e even, 0 when e odd else
 *            if p ≡ 1 or 5 (mod 24): b(p^e) = e + 1 else...
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Validated: standard, pass, 96 terms through n = 995 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t factor_value(idx_t prime, idx_t exponent)
{
    /* Prime powers of 2 or 3 always give zero */
    if (prime == 2 || prime == 3)
        return 0;

    /* p ≡ 3 (mod 4)  (p ≠ 3, so p = 3,7,11,19,23,… but p==3 handled above) */
    if (prime % 4 == 3)
        return (exponent % 2 == 0) ? 1 : 0;

    /* Remaining p are ≡ 1 (mod 4).  They belong to 4 residue classes mod 24:
       1, 5, 13, 17.  Use the appropriate rule. */
    int r = (int)(prime % 24);
    if (r == 1 || r == 5)            /* p ≡ 1 or 5 (mod 24) */
        return (seq_t)(exponent + 1);

    if (r == 13 || r == 17) {        /* p ≡ 13 or 17 (mod 24) */
        seq_t val = (seq_t)(exponent + 1);
        if (exponent % 2 == 1)      /* odd exponent -> sign change */
            val = -val;
        return val;
    }

    /* All other residues do not occur for primes, but fall back to zero */
    return 0;
}

seq_t a(idx_t n)
{
    idx_t m = n * 4ULL + 1;     /* argument of b: 4*n + 1 */
    seq_t result = 1;

    idx_t temp = m;
    /* Trial division up to sqrt(temp).  Use p <= temp / p to avoid overflow. */
    for (idx_t p = 2; p <= temp / p; ++p) {
        if (temp % p == 0) {
            idx_t exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            seq_t f = factor_value(p, exp);
            if (f == 0)
                return 0;          /* multiplicative zero propagates */
            result *= f;
        }
    }
    if (temp > 1) {                /* remaining prime factor with exponent 1 */
        seq_t f = factor_value(temp, 1);
        if (f == 0)
            return 0;
        result *= f;
    }
    return result;
}
