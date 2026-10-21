/*
 * A228443  G.f.: Sum_{k>=0} (2*k + 1) * x^k / (1 + x^(2*k + 1)).
 * https://oeis.org/A228443
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = b(2n+1) where b is multiplicative: b(2^e)=0^e, b(p^e) =
 *            (p^(e+1)-1)/(p-1) when p≡1 (mod 4), b(p^e) =
 *            (p^(e+1)+(-1)^e)/(p+1) when p≡3 (mod 4)
 * Range:     u8 n <= 8, u16 n <= 1545, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A228443 */
#include <stdint.h>

/* Helper: compute the factor contribution for a prime power p^e */
static seq_t prime_power_factor(idx_t p, int e)
{
    if ((p & 3) == 1) {              /* p ≡ 1 (mod 4) */
        seq_t sum = (seq_t)1;
        seq_t term = (seq_t)1;
        for (int i = 0; i < e; ++i) {
            term *= (seq_t)p;
            sum += term;
        }
        return sum;
    } else {                          /* p ≡ 3 (mod 4) */
        seq_t pow = (seq_t)1;
        for (int i = 0; i < e + 1; ++i)
            pow *= (seq_t)p;
        seq_t numerator = ((e & 1) == 0) ? (pow + (seq_t)1) : (pow - (seq_t)1);
        return numerator / ((seq_t)p + (seq_t)1);
    }
}

seq_t a(idx_t n)
{
    idx_t m = 2 * n + 1;
    if (m == 1) return (seq_t)1;

    seq_t result = (seq_t)1;
    idx_t temp = m;

    /* factorization by trial division (m is odd) */
    for (idx_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            int e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            result *= prime_power_factor(p, e);
        }
    }

    if (temp > 1) {                    /* remaining prime factor */
        result *= prime_power_factor(temp, 1);
    }

    return result;
}
