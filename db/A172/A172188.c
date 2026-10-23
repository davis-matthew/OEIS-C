/*
 * A172188  Partial sums of primes of the form 3*k-1.
 * https://oeis.org/A172188
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of the first n primes p with p ≡ 2 (mod 3)
 * Range:     u8 n <= 9, u16 n <= 109, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* That is, primes of the form 3*k‑1, k ≥ 1. */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* External linkage as required */
seq_t a(idx_t n)
{
    /* Static cache of partial sums, primes used for trial division,
       and the next candidate prime to test. */
    static seq_t *sums      = NULL;   /* 1-indexed: sums[1] = 2, ... */
    static size_t sums_cap  = 0;       /* capacity of sums (number of terms) */
    static size_t sums_len  = 0;       /* number of terms currently stored */
    static uint64_t *primes   = NULL;   /* list of all primes found so far */
    static size_t primes_cap  = 0;      /* capacity of primes array   */
    static size_t primes_len  = 0;      /* number of primes stored    */
    static uint64_t next_candidate = 2; /* next integer to test for primality */

    /* Ensure we have computed at least n terms. */
    while (sums_len < (size_t)n)
    {
        /* Find the next prime candidate. */
        for (;;)
        {
            /* Trial division using the primes list. */
            bool isprime = true;
            for (size_t i = 0; i < primes_len; ++i)
            {
                uint64_t p = primes[i];
                if (p * p > next_candidate)
                    break;
                if (next_candidate % p == 0)
                {
                    isprime = false;
                    break;
                }
            }

            if (isprime)
            {
                /* Store the prime in the primes list. */
                if (primes_len == primes_cap)
                {
                    primes_cap = primes_cap ? primes_cap * 2 : 4;
                    primes = (uint64_t *)realloc(primes, primes_cap * sizeof(uint64_t));
                }
                primes[primes_len++] = next_candidate;

                /* If it is of the required form (≡ 2 mod 3), add to sums. */
                if (next_candidate % 3 == 2)
                {
                    seq_t new_sum = sums_len == 0
                                    ? (seq_t)next_candidate
                                    : sums[sums_len] + (seq_t)next_candidate;

                    if (sums_len == sums_cap)
                    {
                        sums_cap = sums_cap ? sums_cap * 2 : 4;
                        sums = (seq_t *)realloc(sums, (sums_cap + 1) * sizeof(seq_t));
                        sums[0] = (seq_t)0; /* dummy entry for 1‑based indexing */
                    }
                    ++sums_len;
                    sums[sums_len] = new_sum;
                    ++next_candidate;          /* advance to next integer */
                    break;                     /* new term added; exit inner loop */
                }
            }
            ++next_candidate;  /* not prime or not our form: test next integer */
        }
    }

    /* Return the cached value.  The array is 1‑based. */
    return sums[(size_t)n];
}
