/*
 * A122855  Expansion of (phi(q^3)phi(q^5) + phi(q)phi(q^15))/2 in powers of q
 *          where phi(q) is a Ramanujan theta function.
 * https://oeis.org/A122855
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   not stated
 * Range:     u8 n <= 5000, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Multiplicative function defined by 
   a(0) = 1,
   a(2^e) = |e-1|,
   a(3^e) = a(5^e) = 1,
   a(p^e) = e+1          if p ≡ 1,2,4,8 (mod 15),
   a(p^e) = (1+(-1)^e)/2 if p ≡ 7,11,13,14 (mod 15).
   This matches OEIS A122855.
*/

seq_t a(idx_t n)
{
    if (n == 0) return 1;

    idx_t tmp = n;
    seq_t result = 1;

    /* factor 2 */
    idx_t e = 0;
    while ((tmp & 1) == 0) { tmp >>= 1; ++e; }
    if (e > 0) {
        seq_t val = (e == 1 ? 0 : e - 1);
        result *= val;
    }

    /* factor 3 */
    e = 0;
    while (tmp % 3 == 0) { tmp /= 3; ++e; }
    /* value is 1, nothing to multiply */

    /* factor 5 */
    e = 0;
    while (tmp % 5 == 0) { tmp /= 5; ++e; }
    /* value is 1, nothing to multiply */

    /* factor odd primes >= 7 */
    for (idx_t p = 7; p <= tmp / p; p += 2) {
        if (tmp % p == 0) {
            e = 0;
            while (tmp % p == 0) { tmp /= p; ++e; }

            if (p % 15 == 1 || p % 15 == 2 || p % 15 == 4 || p % 15 == 8) {
                result *= (seq_t)(e + 1);
            } else { /* residues 7,11,13,14 */
                if (e & 1) {          /* odd exponent -> zero */
                    return 0;
                }
                /* even exponent -> value 1, nothing to multiply */
            }
        }
    }

    /* leftover prime factor > sqrt(original n) */
    if (tmp > 1) {
        idx_t p = tmp; /* it's prime, exponent is 1 */
        if (p % 15 == 1 || p % 15 == 2 || p % 15 == 4 || p % 15 == 8) {
            result *= 2;   /* e + 1 = 1 + 1 */
        } else if (p % 15 == 7 || p % 15 == 11 || p % 15 == 13 || p % 15 == 14) {
            return 0;     /* odd exponent (1) gives zero */
        }
        /* primes 3 and 5 were already handled */
    }

    return result;
}
