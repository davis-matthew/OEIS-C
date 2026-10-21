/*
 * A079277  Largest integer k < n such that any prime factor of k is also a
 *          prime factor of n.
 * https://oeis.org/A079277
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = largest k < n such that every prime factor of k also
 *            divides n (i.e., rad(k) | rad(n)). Implementation uses a
 *            straightforward downward search
 * Range:     u8 n <= 257, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;           /* undefined for n < 2 – not used */

    /* Step 1: collect the distinct prime factors of n.  */
    idx_t primes[64];
    idx_t pcount = 0;
    idx_t tmp = n;

    for (idx_t p = 2; p <= tmp / p; ++p) {
        if (tmp % p == 0) {
            primes[pcount++] = p;
            while (tmp % p == 0) tmp /= p;
        }
    }
    if (tmp > 1)            /* remaining prime factor of n */
        primes[pcount++] = tmp;

    /* Step 2: search downward from n‑1 for the largest admissible k. */
    for (idx_t k = n - 1; k > 0; --k) {
        idx_t v = k;
        for (idx_t i = 0; i < pcount; ++i) {
            idx_t p = primes[i];
            while (v % p == 0) v /= p;   /* strip all powers of p */
        }
        if (v == 1)                  /* all prime factors are in the set */
            return (seq_t)k;
    }

    return (seq_t)1;   /* unreachable: k = 1 always satisfies the condition */
}
