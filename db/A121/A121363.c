/*
 * A121363  Expansion of
 *          q^(-1/4)(eta(q)*eta(q^6)*eta(q^9)/eta(q^3))^2/(eta(q^2)eta(q^18))
 *          in powers of q.
 * https://oeis.org/A121363
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = b(4n+1) where b is a multiplicative function defined by
 *            b(2^e) = 0^e, b(3^e) = 0^e, b(p^e) = (1+(-1)^e)/2 if p ≡ 3 (mod
 *            4), b(p^e) = e+1 if p ≡ 1 (mod 12), b(p^e) = (e+1)(-1)^e if p ≡
 *            5 (mod 12)
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* m = 4n + 1 */
    idx_t m = 4 * n + 1;

    /* If m is divisible by 2 or 3, b(m) = 0 */
    if ((m & 1ULL) == 0 || (m % 3ULL) == 0)
        return 0;

    seq_t prod = 1;
    idx_t temp = m;

    /* Factorization by trial division */
    for (idx_t p = 5; p * p <= temp; p += 2) {
        if (temp % p != 0)
            continue;

        idx_t exp = 0;
        while (temp % p == 0) {
            temp /= p;
            ++exp;
        }

        seq_t factor = 1;

        if (p % 4ULL == 3ULL) {          /* p ≡ 3 (mod 4) (p ≠ 3) */
            factor = (exp & 1ULL) ? 0 : 1;
        } else if (p % 12ULL == 1ULL) {   /* p ≡ 1 (mod 12) */
            factor = (seq_t)(exp + 1ULL);
        } else if (p % 12ULL == 5ULL) {   /* p ≡ 5 (mod 12) */
            seq_t val = (seq_t)(exp + 1ULL);
            factor = (exp & 1ULL) ? -val : val;
        } else { /* p ≡ 7 or 11 mod 12: already handled by p%4==3 */
            factor = 1;
        }

        if (factor == 0)
            return 0;

        prod *= factor;
    }

    /* Remaining prime factor > sqrt(m) */
    if (temp > 1) {
        idx_t p = temp;
        idx_t exp = 1;
        seq_t factor = 1;

        if (p % 4ULL == 3ULL) {
            factor = 1;   /* p ≠ 3, so exp is 1 (odd) -> factor 0? */
            /* For primes ≡ 3 mod 4 (greater than 3), b(p) = (1+(-1))/2 = 0 */
            factor = 0;
        } else if (p % 12ULL == 1ULL) {
            factor = (seq_t)(exp + 1ULL);
        } else if (p % 12ULL == 5ULL) {
            seq_t val = (seq_t)(exp + 1ULL);
            factor = (exp & 1ULL) ? -val : val;
        } else { /* p ≡ 7 or 11 (mod 12), handled above */
            factor = 1;
        }

        if (factor == 0)
            return 0;

        prod *= factor;
    }

    return prod;
}
