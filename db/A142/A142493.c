/*
 * A142493  Primes congruent to 28 mod 51.
 * https://oeis.org/A142493
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 180, u32 n <= 205, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A142493 – primes p such that p ≡ 28 (mod 51) */
/*   a(n) = the n-th prime of the form 51*k + 28 (1‑based) */

#include "../utils/is_prime_big.h"   /* bool is_prime_big(seq_t) */

/* Return the n‑th prime congruent to 28 modulo 51.
   n is 1‑based; for n < 1 the function returns 0.          */
seq_t a(idx_t n)
{
    if (n < 1) return 0;

    idx_t    k    = 0;                /* k in 51*k + 28 */
    seq_t    count = 0;               /* number of primes found so far */
    seq_t    p;                       /* current candidate */

    while (1) {
        p = (seq_t)51 * (seq_t)k + 28;  /* candidate of the form 51k+28 */

        /* 28 is not prime; the first valid candidate is 79 (k=1).  */
        if (is_prime_big(p)) {
            count++;
            if (count == n)
                return p;
        }
        k++;
    }
}
