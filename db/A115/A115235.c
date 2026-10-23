/*
 * A115235  Expansion of eta(q)^2*eta(q^9)*eta(q^18)/(eta(q^2)*eta(q^3)) in
 *          powers of q.
 * https://oeis.org/A115235
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, mult
 * Formula:   A115235 is multiplicative with * a(2^e) = (-1 + 3*(-1)^e)/2 *
 *            a(3^e) = 0 for e>0 (here we short‑circuit any n divisible by 3)
 *            * a(p^e) = e+1 if p ≡ 1 (mod 6) * a(p^e) = (1+(-1)^e)/2 if p ≡ 5
 *            (mod 6) * The implementation factors n and multiplies the
 *            corresponding local * contributions. It runs in...
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* 0 has no defined value in OEIS; for safety return 0 */
    if (n == 0) return (seq_t)0;

    /* Any multiple of 3 gives 0 because a(3^e)=0 for e>0 */
    if (n % 3 == 0) return (seq_t)0;

    seq_t result = (seq_t)1;
    idx_t temp   = n;

    /* Handle factor 2 */
    if ((temp & 1UL) == 0) {
        idx_t e = 0;
        while ((temp & 1UL) == 0) {
            ++e;
            temp >>= 1;          /* divide by 2 */
        }
        if (e & 1) {
            result *= (seq_t)-2; /* odd exponent → factor -2 */
        }                           /* even exponent → factor 1 (do nothing) */
    }

    /* Iterate over odd primes up to sqrt(temp) */
    for (idx_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            idx_t exp = 0;
            while (temp % p == 0) {
                ++exp;
                temp /= p;
            }

            if (p % 6 == 1) {
                result *= (seq_t)(exp + 1);      /* e+1 */
            } else if (p % 6 == 5) {
                if (exp & 1)                    /* odd exponent → factor 0 */
                    return (seq_t)0;
                /* even exponent → factor 1 (do nothing) */
            }
            /* p==3 never occurs here because n%3==0 was handled earlier */
        }
    }

    /* Remaining prime factor, if any */
    if (temp > 1) {
        /* temp is now an odd prime > 1 */
        if (temp % 6 == 1) {
            result *= (seq_t)2;      /* exponent = 1 → e+1 = 2 */
        } else if (temp % 6 == 5) {
            return (seq_t)0;          /* odd exponent (1) gives factor 0 */
        }
    }

    return result;
}
