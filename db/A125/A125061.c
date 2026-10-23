/*
 * A125061  Expansion of psi(q) * psi(q^2) * chi(q^3) * chi(-q^6) in powers of
 *          q where psi(), chi() are Ramanujan theta functions.
 * https://oeis.org/A125061
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) is multiplicative with
 *   a(0) = 1.
 *   a(2^e) = 1,
 *   a(3^e) = 2 - (-1)^e,
 *   a(p^e) = e + 1        if p ≡ 1 (mod 4) and p > 3,
 *   a(p^e) = 0            if p ≡ 3 (mod 4) and e is odd,
 *   a(p^e) = 1            if p ≡ 3 (mod 4) and e is even,
 *   (p > 3)
 */

/* Return the n-th term of A125061. */
seq_t a(idx_t n)
{
    if (n == 0) return 1;

    /* Use a temporary variable for factorisation. */
    idx_t m = n;
    seq_t res = 1;

    /* Handle factor 2: its contribution is always 1. */
    while ((m & 1) == 0)           /* m % 2 == 0 */
        m >>= 1;                   /* m /= 2 */

    /* Handle prime 3 specially. */
    unsigned e3 = 0;
    while (m % 3 == 0) {
        ++e3;
        m /= 3;
    }
    if (e3 & 1) /* odd exponent */
        res *= 3u;          /* multiply by 3 */

    /* Factor remaining odd primes >=5. */
    for (idx_t p = 5; p * p <= m; p += 2) {
        if (m % p == 0) {
            unsigned e = 0;
            while (m % p == 0) {
                ++e;
                m /= p;
            }

            if (p % 4 == 1) {
                /* p ≡ 1 (mod 4) */
                res *= (seq_t)(e + 1);
            } else if (p % 4 == 3) {
                /* p ≡ 3 (mod 4) */
                if (e & 1)          /* odd exponent -> zero */
                    return 0;
                /* even exponent gives factor 1, nothing to do */
            }
        }
    }

    /* If m > 1, it is a remaining prime factor. */
    if (m > 1) {
        if (m % 4 == 1) {
            res *= 2u;             /* exponent 1 → e+1 = 2 */
        } else if (m % 4 == 3) {
            /* exponent 1 odd → zero */
            return 0;
        }
    }

    return res;
}
