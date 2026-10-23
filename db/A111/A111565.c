/*
 * A111565  Largest prime factor of prime power > 1 that divides the n-th
 *          composite number; or a(n) = 0 iff n-th composite number is equal
 *          to the product of distinct primes.
 * https://oeis.org/A111565
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 102, u16 n <= 102, u32 n <= 102, u64 n <= 102,
 *            u128 n <= 102, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) of A111565: for the n‑th composite number C
   return the largest prime that occurs to a power > 1 in C;
   if all exponents in the factorisation are 1, return 0. */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* safety: treat n as an unsigned 64‑bit value for the loop */
    uint64_t target = (uint64_t)n;
    uint64_t count = 0;      /* how many composites seen so far */
    uint64_t num   = 4;      /* first composite number */

    for (;; ++num) {
        if (!is_prime(num)) {          /* found a composite */
            ++count;
            if (count == target) {     /* this is the n‑th composite */
                uint64_t temp = num;
                seq_t largest = 0;

                /* factor 2 separately */
                if ((temp & 1ULL) == 0) {
                    uint64_t exp = 0;
                    while ((temp & 1ULL) == 0) {
                        temp >>= 1;
                        ++exp;
                    }
                    if (exp >= 2) largest = 2;
                }

                /* factor odd primes */
                for (uint64_t p = 3; p * p <= temp; p += 2) {
                    if (temp % p == 0) {
                        uint64_t exp = 0;
                        while (temp % p == 0) {
                            temp /= p;
                            ++exp;
                        }
                        if (exp >= 2 && p > largest)
                            largest = p;
                    }
                }

                /* any remaining prime factor has exponent 1 and is ignored */
                return (seq_t)largest;   /* 0 if none found */
            }
        }
    }
}
