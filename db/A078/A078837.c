/*
 * A078837  a(n) = Sum_{k=1..prime(n)-1} floor(k^3/prime(n)).
 * https://oeis.org/A078837
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((prime(n)-2)*(prime(n)-1)*(prime(n)+1))/4, where
 *            prime(n) is the nth prime (n ≥ 1)
 * Range:     u8 n <= 4, u16 n <= 13, u32 n <= 285, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 51 terms through n = 927 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
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

seq_t a(idx_t n)
{
    /* Fetch the nth prime.  */
    seq_t p = nth_prime(n);

    /* Compute (p-2)*(p-1)*(p+1) and divide by 4.  */
    seq_t t1 = p - 2;
    seq_t t2 = p - 1;
    seq_t t3 = p + 1;

    seq_t prod = t1 * t2 * t3;
    return prod / 4;
}
