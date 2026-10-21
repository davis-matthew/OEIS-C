/*
 * A226162  a(n) = Kronecker Symbol (-5/n), n >= 0.
 * https://oeis.org/A226162
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) = Kronecker symbol (-5/n), completely multiplicative. * *
 *            a(0) = 0 * a(1) = 1 * a(2) = -1 * a(5) = 0 * For odd primes p ≠
 *            5: * a(p) = 1 if p ≡ 1,3,7,9 (mod 20) * a(p) = -1 if p ≡
 *            11,13,17,19 (mod 20) * The value for general n is the product of
 *            its prime‑power values, * with a(p^e) = a(p)^e....
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)           /* offset 0 */
        return 0;

    seq_t res = 1;
    uint64_t m = n;       /* work on unsigned 64-bit value */

    /* any factor 5 gives symbol 0 */
    if ((m % 5ULL) == 0ULL)
        return 0;

    /* handle factor 2 */
    if ((m & 1ULL) == 0ULL) {
        uint64_t e2 = 0;
        while ((m & 1ULL) == 0ULL) {
            ++e2;
            m >>= 1;
        }
        if (e2 & 1ULL)        /* odd exponent -> multiply by a(2) = -1 */
            res = -res;
    }

    /* factor odd primes */
    for (uint64_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0ULL) {
            uint64_t ep = 0;
            while (m % p == 0ULL) {
                ++ep;
                m /= p;
            }
            /* p is odd and not 5, compute a(p) from its residue mod 20 */
            int aP = 1;
            switch (p % 20ULL) {
                case 11: case 13: case 17: case 19: aP = -1; break;
                default:               /* 1,3,7,9, etc. */ break;
            }
            if (ep & 1ULL)          /* odd exponent */
                res *= (seq_t)aP;   /* multiply by aP = ±1 */
        }
    }

    /* any remaining factor is a prime > sqrt(original n) */
    if (m > 1ULL) {
        uint64_t p = m;
        /* p cannot be 5 because that was tested above */
        int aP = 1;
        switch (p % 20ULL) {
            case 11: case 13: case 17: case 19: aP = -1; break;
            default:               /* 1,3,7,9, etc. */ break;
        }
        res *= (seq_t)aP; /* exponent is 1 (odd) */
    }

    return res;
}
