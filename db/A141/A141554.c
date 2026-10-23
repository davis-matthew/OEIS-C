/*
 * A141554  Transformed nonprime products of prime factors of the composites,
 *          the largest prime decremented by 2 and the smallest prime
 *          incremented by 2.
 * https://oeis.org/A141554
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = transformed product of the prime factors of the n‑th
 *            composite number (A002808). One occurrence of the largest prime
 *            factor pmax is replaced by pmax‑2, one occurrence of the
 *            smallest prime factor pmin is replaced by pmin+2. If the
 *            resulting value is prime it is skipped; the sequence consis...
 * Range:     u8 n <= 5, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* ---- helper functions ---------------------------------------------------- */

/* Integer primality test (trial division) for 64‑bit values. */
static inline bool isPrime(uint64_t x)
{
    if (x < 2) return false;
    if (x % 2 == 0) return x == 2;
    for (uint64_t d = 3; d * d <= x; d += 2) {
        if (x % d == 0) return false;
    }
    return true;
}

/* Smallest prime factor of n (n > 1).  Called only for composite n. */
static inline uint64_t spf(uint64_t n)
{
    if (n % 2 == 0) return 2;
    for (uint64_t d = 3; d * d <= n; d += 2) {
        if (n % d == 0) return d;
    }
    return n;           /* n is prime, but this should not happen here */
}

/* ---- main sequence function ---------------------------------------------- */

seq_t a(idx_t n)
{
    idx_t found = 0;                /* number of sequence terms found */
    uint64_t k = 4;                 /* first composite number */

    while (1) {
        /* skip primes – we only transform composites */
        if (isPrime(k)) { k++; continue; }

        /* find smallest and largest prime factors */
        uint64_t pmin = spf(k);
        uint64_t temp = k;
        uint64_t pmax = 0;
        while (temp > 1) {
            uint64_t f = spf(temp);
            if (f > pmax) pmax = f;
            temp /= f;
        }

        /* compute transformed value: k * (pmax-2) * (pmin+2) / (pmax*pmin) */
        seq_t val = (seq_t)k * ((seq_t)pmax - 2) * ((seq_t)pmin + 2);
        seq_t denom = (seq_t)pmin * (seq_t)pmax;
        val /= denom;

        if (!isPrime((uint64_t)val)) {          /* skip primes (val > 1) */
            found++;
            if (found == n) return val;
        }

        k++;   /* next integer to check */
    }
}
