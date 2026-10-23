/*
 * A171044  In the sequence of prime numbers, replace all the '8' digits with
 *          '3' and vice versa.
 * https://oeis.org/A171044
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = replace digits 3 ↔ 8 in the n‑th prime number
 * Range:     u8 n <= 50, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   replace_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/replace_digits.h"

/* Maximum number of primes that will be generated.
   n ≤ 10000 in the tests, so 20000 is safely large. */
#define MAX_PRIMES 20000

static seq_t prime_list[MAX_PRIMES];
static idx_t prime_count = 0;

/* Generate all primes up to the requested index n.
   The function keeps the list in prime_list.  */
static void ensure_primes(idx_t n)
{
    if (prime_count >= n) return;

    /* start generating from the last known prime or from 2 */
    idx_t cand = 2;
    if (prime_count > 0) cand = (idx_t)prime_list[prime_count - 1] + 1;
    if (cand > 2 && cand % 2 == 0) cand++;      /* first odd after even */

    while (prime_count < n) {
        /* Test whether cand is prime.  */
        bool isprime = true;
        for (idx_t i = 0; i < prime_count; ++i) {
            seq_t p = prime_list[i];
            if ((idx_t)p * (idx_t)p > cand) break;
            if (cand % (idx_t)p == 0) { isprime = false; break; }
        }

        if (isprime) {
            if (prime_count < MAX_PRIMES) {
                prime_list[prime_count++] = (seq_t)cand;
            } else {
                /* Should never happen for the intended usage. */
                return;
            }
        }

        cand += 2;          /* only odd candidates after 2 */
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset starts at 1, undefined for 0 */

    ensure_primes(n);
    seq_t prime = prime_list[n - 1];
    return replace_digits(prime);
}
