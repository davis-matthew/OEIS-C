/*
 * A123530  Expansion of q^(-1/2)*eta(q)^2*eta(q^6)^3/(eta(q^2)*eta(q^3)^2) in
 *          powers of q.
 * https://oeis.org/A123530
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = b(2n+1) where b is multiplicative: b(3^e) = -2 (for any
 *            e>0), b(p^e) = e+1 if p ≡ 1 (mod 6), b(p^e) = (1+(-1)^e)/2 if p
 *            ≡ 5 (mod 6). a(n) = 0 when any factor with p ≡ 5 (mod 6) has odd
 *            exponent
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* We evaluate b(2n+1).  The argument is always odd. */
    idx_t m = 2 * n + 1;
    seq_t result = 1;

    idx_t cur = m;
    idx_t p   = 3;
    while (p * p <= cur) {
        if (cur % p == 0) {
            /* Count exponent of p */
            idx_t exp = 0;
            while (cur % p == 0) {
                cur /= p;
                ++exp;
            }

            if (p == 3) {
                /* For 3^e, factor is always -2 */
                result *= (seq_t)(-2);
            } else if (p % 6 == 1) {
                /* p ≡ 1 (mod 6): factor is e+1 */
                result *= (seq_t)(exp + 1);
            } else {  /* p % 6 == 5 for primes > 3 */
                if ((exp & 1) != 0) {
                    /* odd exponent gives factor 0 */
                    return (seq_t)0;
                }
                /* even exponent gives factor 1 – nothing to do */
            }
            if (result == 0) return (seq_t)0; /* early exit if already zero */
        }
        p += 2;   /* next odd candidate */
    }

    if (cur > 1) {   /* remaining prime factor */
        idx_t p = cur;
        if (p == 3) {
            result *= (seq_t)(-2);
        } else if (p % 6 == 1) {
            /* exponent is 1 → e+1 = 2 */
            result *= (seq_t)2;
        } else { /* p % 6 == 5 */
            /* exponent 1 is odd → factor 0 */
            return (seq_t)0;
        }
    }

    return result;
}
