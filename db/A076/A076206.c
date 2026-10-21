/*
 * A076206  Numbers with digital root equal to their number of divisors.
 * https://oeis.org/A076206
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 4894, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search implementation for the sequence A076206:
   Numbers whose digital root equals the number of divisors (τ(n)). */
/* This algorithm scans successive integers, computes τ(n) by trial division,
   checks the digital root, and stores the results in a dynamic array. */

#include <stddef.h>
#include <stdlib.h>

/* The build supplies the integer types seq_t and idx_t. */
static seq_t *terms = NULL;    /* dynamic array of found terms          */
static idx_t   terms_sz = 0;   /* number of terms stored               */
static idx_t   terms_cap = 0;  /* current capacity of the array         */
static idx_t   next_candidate = 1;   /* next integer to test                 */

/* Return the next index in `terms` to store a found term.  */
static void grow_terms(void)
{
    if (terms_cap == 0) {
        terms_cap = 64;
        terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
    } else if (terms_sz == terms_cap) {
        terms_cap *= 2;
        terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
    }
}

/* Compute the digital root of a non‑negative integer m in O(1). */
static idx_t digital_root(idx_t m)
{
    if (m == 0)
        return 0;
    return (m - 1) % 9 + 1;
}

/* Compute the number of divisors τ(m) with an early exit if τ>9. */
static int divisor_count(idx_t m)
{
    idx_t n = m;
    int tau = 1;

    /* Count factor 2 separately. */
    if ((n & 1) == 0) {
        int exp = 0;
        while ((n & 1) == 0) {
            n >>= 1;
            exp++;
        }
        tau *= (exp + 1);
        if (tau > 9)
            return tau; /* early exit */
    }

    /* Trial division by odd primes. */
    for (idx_t p = 3; p * p <= n; p += 2) {
        if (n % p == 0) {
            int exp = 0;
            while (n % p == 0) {
                n /= p;
                exp++;
            }
            tau *= (exp + 1);
            if (tau > 9)
                return tau; /* early exit */
        }
    }

    /* Remaining prime factor > sqrt(original). */
    if (n > 1)
        tau *= 2;

    return tau;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined for n=0, but safeguard */

    /* Initialise the dynamic array on first call. */
    if (terms == NULL)
        grow_terms();

    /* Generate terms until we have at least n of them. */
    while (terms_sz < n) {
        /* Check the current candidate. */
        idx_t m = next_candidate;

        /* For m == 1 the divisor count is 1 and digital root is 1. */
        int tau = (m == 1) ? 1 : divisor_count(m);
        idx_t dr = digital_root(m);

        if (tau == dr) {
            grow_terms();
            terms[terms_sz++] = (seq_t)m;
        }

        /* Prepare next candidate. */
        next_candidate++;
    }

    /* Return the (n-1)th stored term (sequence is 1‑indexed). */
    return terms[n - 1];
}
