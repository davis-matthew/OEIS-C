/*
 * A174238  Inverse Moebius transform of even part of n (A006519).
 * https://oeis.org/A174238
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = (2^(v2(n)+1)-1) * d(n/2^v2(n)) where v2(n) is the
 *            exponent of 2 in n and d(k) is the number of divisors of k
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Special case: n == 0 is not defined, but handle gracefully */
    if (n == 0) return 0;

    /* Compute exponent of 2 and odd part of n */
    idx_t e = 0;
    idx_t odd = n;
    while ((odd & 1) == 0) {
        ++e;
        odd >>= 1;
    }

    /* Compute divisor count d(odd).  Only odd part remains. */
    seq_t d = 1;
    idx_t temp = odd;
    for (idx_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            idx_t exp = 0;
            while (temp % p == 0) {
                ++exp;
                temp /= p;
            }
            d *= (seq_t)(exp + 1);
        }
    }
    if (temp > 1) {          /* remaining prime factor */
        d *= 2;
    }

    /* Compute 2^(e+1)-1 */
    seq_t pow2 = 1;
    for (idx_t i = 0; i < e + 1; ++i) {
        pow2 *= 2;
    }
    pow2 -= 1;

    return pow2 * d;
}
