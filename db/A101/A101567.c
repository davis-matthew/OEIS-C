/*
 * A101567  Numbers n such that 1009*n + 10007 is prime.
 * https://oeis.org/A101567
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth n such that 1009*n + 10007 is prime (offset 1)
 * Range:     u8 n <= 25, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function searches linearly for the n‑th such n, testing primality with is_prime64. */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime64.h"   /* provides: bool is_prime64(uint64_t n); */

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;   /* undefined for n < 1, return 0 as a fallback */

    seq_t m = 0;          /* candidate n   */
    idx_t found = 0;      /* how many primes found so far */

    while (1) {
        uint64_t p = 1009ull * (uint64_t)m + 10007ull; /* candidate prime value */

        if (is_prime64(p)) {
            found++;
            if (found == n) {
                return m;  /* m fits into seq_t for all tested n */
            }
        }
        m++;                /* move to next candidate n */
    }
}
