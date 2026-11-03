/*
 * A098998  p(11p-7) where p is prime.
 * https://oeis.org/A098998
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 21, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Validated: partial, pass, 60 terms through n = 60 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = p_n * (11 * p_n - 7) where p_n is the nth prime
 * (OEIS A098998, offset 1).
 */

#include <stdlib.h>
#include <stddef.h>

/* Return the nth prime (1‑based). */
static idx_t nth_prime(idx_t n)
{
    static idx_t *primes = NULL;   /* array of primes found so far          */
    static size_t size   = 0;      /* number of primes stored               */
    static size_t cap    = 0;      /* allocated capacity of primes array    */
    static idx_t  last   = 3;      /* next candidate to test (odd numbers) */

    /* Ensure the first prime (2) is stored. */
    if (size == 0) {
        cap = 4;
        primes = (idx_t *)malloc(cap * sizeof(idx_t));
        primes[0] = 2;
        size = 1;
    }

    /* Generate primes until we have at least n of them. */
    while (size < n) {
        idx_t p = last;
        int isprime = 1;
        for (size_t j = 0; j < size; ++j) {
            idx_t pr = primes[j];
            if (pr * pr > p) break;
            if (p % pr == 0) { isprime = 0; break; }
        }
        if (isprime) {
            if (size == cap) {             /* grow the array */
                cap *= 2;
                primes = (idx_t *)realloc(primes, cap * sizeof(idx_t));
            }
            primes[size++] = p;
        }
        last += 2;                           /* skip even numbers */
    }

    return primes[n - 1];                    /* nth prime (1‑based) */
}

/* Compute A098998(n).  n is 1‑based; terms for n==0 are undefined. */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return 0;              /* offset guard */

    idx_t p = nth_prime(n);                   /* n-th prime */
    seq_t p_seq = (seq_t)p;                   /* promote to seq_t */

    /* Compute 11*p^2 - 7*p = p*(11*p - 7) avoiding overflow in uint64_t
       for the intended test ranges. */
    seq_t ans = p_seq * ((p_seq * (seq_t)11) - (seq_t)7);
    return ans;
}
