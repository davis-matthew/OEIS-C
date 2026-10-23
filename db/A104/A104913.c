/*
 * A104913  Primes p such that the string 9876543210p is prime, or primes in
 *          A104912.
 * https://oeis.org/A104913
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   digit_count_uint64, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A104913: primes p such that 9876543210p is prime.  */
/* a(n) = the n-th prime p (n ≥ 1) for which the decimal concatenation
   9876543210 followed by the digits of p yields a prime number. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"      /* is_prime(uint64_t) */
#include "../utils/digit_count_uint64.h"

/* Return the smallest prime strictly greater than cur. */
static uint64_t next_prime_after(uint64_t cur)
{
    uint64_t cand = (cur < 2) ? 2 : cur + 1;
    if (cand == 2)
        return 2;
    if ((cand & 1) == 0)
        cand++;
    for (;;) {
        if (is_prime(cand))
            return cand;
        cand += 2;
    }
}

/* Compute a(n).  The function keeps static state so that repeated
   calls with arbitrary n values are efficient. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined for n=0 but harmless */

    /* static state across calls */
    static size_t found = 0;          /* number of terms found so far   */
    static seq_t *terms = NULL;       /* array of found primes          */
    static size_t capacity = 0;       /* allocated capacity of array    */
    static uint64_t last_checked = 0; /* last prime checked             */
    static bool init = false;
    static uint64_t pow10[20];        /* powers of 10 up to 10^19       */
    const uint64_t PREFIX = 9876543210ULL;  /* constant “9876543210” */

    if (!init) {
        /* one-time initialization of powers of ten */
        pow10[0] = 1ULL;
        for (int i = 1; i < 20; ++i)
            pow10[i] = pow10[i - 1] * 10ULL;
        init = true;
    }

    /* Ensure the storage array is large enough. */
    if (capacity == 0) {
        capacity = 64;
        terms = (seq_t *)malloc(capacity * sizeof(seq_t));
    }

    /* Generate terms until we have at least n of them. */
    while (found < (size_t)n) {
        /* Find next prime candidate p. */
        uint64_t p = next_prime_after(last_checked);
        last_checked = p;

        /* Compute the concatenated number: 9876543210 * 10^d + p. */
        int d = digit_count_uint64(p);
        if (d >= 20) /* safety: too many digits for our pow10 array */
            break;   /* it should not happen for the intended limits */

        /* Use 128‑bit to avoid overflow before the final cast. */
        __int128 concat128 = (__int128)PREFIX * pow10[d] + p;
        /* The concatenated number fits in 64 bits for the range we
           support (p has at most 9 digits). */
        uint64_t concat = (uint64_t)concat128;

        if (is_prime(concat)) {
            /* Store p.  Expand storage if necessary. */
            if (found == capacity) {
                capacity <<= 1;
                terms = (seq_t *)realloc(terms, capacity * sizeof(seq_t));
            }
            terms[found++] = (seq_t)p;
        }
    }

    /* Return the nth term (1‑based index). */
    return terms[n - 1];
}
