/*
 * A222565  Primes that are the largest anti-divisor of primes.
 * https://oeis.org/A222565
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS A222565: primes that are the largest anti‑divisor of primes.
   The sequence consists of
       2 followed by all primes p such that
       • if p ≡ 1 (mod 4) then 6·((p−1)/4)+1 is prime,
       • if p ≡ 3 (mod 4) then 6·((p−3)/4)+5 is prime.
   The implementation iterates over consecutive primes, tests the condition
   with the provided deterministic `is_prime` routine, and returns the
   *n*‑th such prime.  No table of terms is used. */

#include <stdbool.h>
#include "../utils/is_prime.h"   /* provides bool is_prime(idx_t) */

/* Find the next prime ≥ x using the provided is_prime test. */
static idx_t next_prime(idx_t x)
{
    if (x <= 2) return 2;
    if ((x & 1) == 0) ++x;          /* make odd */
    for (;; x += 2) {
        if (is_prime(x)) return x;
    }
}

/* Return the n‑th term of A222565 (1‑based). */
seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t p = 2;                    /* start with the first prime */

    for (;;) {
        if (p == 2) {               /* 2 is always included */
            ++count;
        } else if ((p & 3) == 1) {  /* p ≡ 1 (mod 4) */
            idx_t k = (p - 1) >> 2; /* (p‑1)/4 */
            if (is_prime(6 * k + 1))
                ++count;
        } else {                    /* p ≡ 3 (mod 4) */
            idx_t k = (p - 3) >> 2; /* (p‑3)/4 */
            if (is_prime(6 * k + 5))
                ++count;
        }

        if (count == n)
            return (seq_t)p;        /* found the desired prime */

        p = next_prime(p + 1);      /* move to the next prime */
    }
}
