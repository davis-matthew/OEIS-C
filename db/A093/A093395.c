/*
 * A093395  Numerators of n divided by the product of the anti-divisors of n.
 * https://oeis.org/A093395
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, frac
 * Formula:   not stated
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) from factorization:
   For a given n ≥ 3, let n = 2^e · ∏ p_i^k_i  (p_i odd primes).
   * If n is a power of 2          → a(n) = n.
   * If n has no odd prime (e=0)   → a(n) = n (n is odd prime).
   * If n has exactly one odd prime p^k:
         – if k = 1                → a(n) = p       (includes 2·p case)
         – if k > 1 and k is even  → a(n) = p
         – if k > 1 and k is odd   → a(n) = 1
   * If n has ≥ 2 odd primes       → a(n) = 1.
   This reproduces all the terms of A093395.                                  */
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 3) return (seq_t)n;   /* behaviour below offset is irrelevant */

    idx_t temp = n;
    bool hasTwo = false;
    if ((temp & 1ULL) == 0ULL) {
        hasTwo = true;
        while ((temp & 1ULL) == 0ULL) temp >>= 1;
    }

    idx_t oddCount = 0;
    idx_t oddPrime = 0;   /* only used when oddCount == 1 */
    idx_t oddExp   = 0;

    for (idx_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            oddCount++;
            idx_t exp = 0;
            while (temp % p == 0) {
                temp /= p;
                exp++;
            }
            if (oddCount == 1) {
                oddPrime = p;
                oddExp   = exp;
            }
        }
    }
    if (temp > 1) {
        oddCount++;
        if (oddCount == 1) {
            oddPrime = temp;
            oddExp   = 1;
        }
    }

    if (!hasTwo) { /* odd n */
        if (oddCount == 1) {
            if (oddExp == 1) return (seq_t)n;              /* prime */
            else if ((oddExp & 1ULL) == 0ULL) return (seq_t)oddPrime;
            else return (seq_t)1;
        }
        return (seq_t)1;   /* composite odd with ≥2 distinct odd primes */
    } else { /* even n */
        if (oddCount == 0) return (seq_t)n;               /* power of two */

        if (oddCount == 1) {
            if (oddExp == 1) return (seq_t)oddPrime;      /* 2·p */
            else if ((oddExp & 1ULL) == 0ULL) return (seq_t)oddPrime;
            else return (seq_t)1;
        }
        return (seq_t)1;   /* even number with ≥2 distinct odd primes */
    }
}
