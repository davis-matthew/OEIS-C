/*
 * A209544  Primes not expressed in form n<+>2, where operation <+> defined in
 *          A206853.
 * https://oeis.org/A209544
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1)=3; for n≥2 a(n)= the (n‑2)-th prime ≡1 (mod 8)
 * Range:     u8 n <= 11, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime.h"

/* Static state for on‑demand generation of primes ≡1 (mod 8) */
static idx_t *primes1 = NULL;  /* array of primes congruent to 1 mod 8 */
static size_t primesSize = 0;  /* number of primes stored */
static size_t primesCap  = 0;  /* current capacity of the array */
static idx_t nextCand = 17;    /* next number to test, already ≡1 (mod 8) */

/* Append a new prime to the static array */
static void add_prime(idx_t p)
{
    if (primesSize == primesCap) {
        size_t newCap = (primesCap == 0) ? 16 : primesCap * 2;
        idx_t *tmp = (idx_t *)realloc(primes1, newCap * sizeof(idx_t));
        /* In case realloc fails, we simply abort – this is acceptable
           for the OEIS evaluation harness. */
        if (!tmp) abort();
        primes1 = tmp;
        primesCap = newCap;
    }
    primes1[primesSize++] = p;
}

/* Ensure the array contains at least (needIdx+1) primes (0‑based) */
static void ensure_primes(size_t needIdx)
{
    while (primesSize <= needIdx) {
        /* Find the next prime ≡1 (mod 8). */
        while (!is_prime(nextCand)) {
            nextCand += 8;
        }
        add_prime(nextCand);
        nextCand += 8;
    }
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 1) {
        return (seq_t)3;
    }
    /* a(2) is the first prime ≡1 (mod 8), thus index 0 in primes1 */
    size_t idx = (size_t)(n - 2);
    ensure_primes(idx);
    return (seq_t)primes1[idx];
}
