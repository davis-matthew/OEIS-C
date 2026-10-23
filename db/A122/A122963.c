/*
 * A122963  Triangular numbers with semiprime indices.
 * https://oeis.org/A122963
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000217(A001358(n)) = A001358(n)*(A001358(n)+1)/2
 * Range:     u8 n <= 8, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation builds a sieve of smallest prime factors (SPF) and
   obtains the n‑th semiprime on demand. The result for any n ≥ 1 is
   computed as ((seq_t)m * ((seq_t)m + 1)) / 2, where m is the n‑th semiprime. */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* EXTERNAL: seq_t and idx_t are defined by the build system.
   We use them directly.                                      */

seq_t a(idx_t n)
{
    /* static state for the SPF array and the list of semiprimes */
    static uint64_t *spf = NULL;      /* smallest prime factor of n      */
    static size_t spf_cap = 0;        /* size of spf array                */
    static uint64_t *semiprimes = NULL;   /* semiprimes found so far        */
    static size_t semi_cnt = 0, semi_cap = 0; /* count and capacity          */

    if (n == 0)               /* safety: sequence starts at n=1   */
        return 0;

    /* ------------------------------------------------------------------
       Estimate an upper bound for the n‑th semiprime.  The bound
       n*20+100 comfortably covers n up to 100 000. */
    uint64_t need_limit = n * 20ULL + 100ULL;
    if (need_limit < 4) need_limit = 4;          /* at least 4 */

    /* ------------------------------------------------------------------
       (Re)build the SPF sieve if needed.  We grow to the requested
       limit the first time.  Subsequent calls may only extend it. */
    if (spf_cap < need_limit + 1) {
        uint64_t *new_spf = (uint64_t *)malloc((need_limit + 1) * sizeof(uint64_t));
        for (uint64_t i = 0; i <= need_limit; ++i)
            new_spf[i] = i;

        for (uint64_t i = 2; i * i <= need_limit; ++i) {
            if (new_spf[i] == i) {               /* i is prime */
                for (uint64_t j = i * i; j <= need_limit; j += i)
                    if (new_spf[j] == j)
                        new_spf[j] = i;
            }
        }
        free(spf);
        spf = new_spf;
        spf_cap = need_limit + 1;
    }

    /* ------------------------------------------------------------------
       Generate semiprimes lazily until we have at least n of them. */
    if (semi_cnt < n) {
        uint64_t candidate = (semi_cnt == 0) ? 4ULL : semiprimes[semi_cnt - 1] + 1ULL;

        while (semi_cnt < n) {
            /* Guarantee that the candidate is inside the current SPF array */
            if (candidate >= spf_cap) {
                uint64_t new_limit = spf_cap * 2ULL;
                if (new_limit <= candidate) new_limit = candidate * 2ULL + 100;
                uint64_t *tmp = (uint64_t *)malloc((new_limit + 1) * sizeof(uint64_t));
                for (uint64_t i = 0; i <= new_limit; ++i)
                    tmp[i] = i;

                for (uint64_t i = 2; i * i <= new_limit; ++i) {
                    if (tmp[i] == i) {
                        for (uint64_t j = i * i; j <= new_limit; j += i)
                            if (tmp[j] == j)
                                tmp[j] = i;
                    }
                }
                free(spf);
                spf = tmp;
                spf_cap = new_limit + 1;
            }

            uint64_t x = candidate;
            int factors = 0;
            while (x > 1 && factors <= 2) {
                uint64_t p = spf[x];
                ++factors;
                x /= p;                     /* count multiplicity, not distinct primes */
            }

            if (factors == 2) {             /* semiprime found */
                if (semi_cap <= semi_cnt) {
                    semi_cap = (semi_cap == 0) ? 16 : semi_cap * 2;
                    semiprimes = (uint64_t *)realloc(semiprimes,
                                                     semi_cap * sizeof(uint64_t));
                }
                semiprimes[semi_cnt++] = candidate;
            }
            ++candidate;
        }
    }

    /* ------------------------------------------------------------------
       Retrieve the n‑th semiprime and compute its triangular number. */
    uint64_t m = semiprimes[n - 1];
    seq_t result = ((seq_t)m * ((seq_t)m + 1)) / 2;
    return result;
}
