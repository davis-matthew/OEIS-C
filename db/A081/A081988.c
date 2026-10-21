/*
 * A081988  Product of digits + 1 is a prime.
 * https://oeis.org/A081988
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 100, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_prod_local, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A081988 – numbers n such that the product of their digits + 1 is prime. */
/* The function scans successive integers and counts those that satisfy the condition. */
/* A small helper digit_prod_local is implemented to avoid the buggy external version. */

#include "../utils/is_prime_big.h"
#include "../utils/digit_prod_local.h"

seq_t a(idx_t n)
{
    /* n is 1‑based. This function works for any order of calls, but a simple
       linear search is sufficient for the required range (n up to ~10000). */
    idx_t count = 0;          /* number of found terms so far */
    seq_t cand = (seq_t)1;    /* candidate integer to test */

    for (;; ++cand) {
        seq_t p = digit_prod_local(cand) + (seq_t)1;
        if (is_prime_big(p)) {
            ++count;
            if (count == n)
                return cand;
        }
    }
}
