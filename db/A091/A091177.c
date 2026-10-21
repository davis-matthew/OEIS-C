/*
 * A091177  Numbers m such that the m-th prime is of the form 3*k-1.
 * https://oeis.org/A091177
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 132, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A091177: indices of primes congruent to 2 (mod 3).
   Computation: primes are generated incrementally by trial division,
   recording the prime index whenever the prime ≡ 2 (mod 3).  The static
   tables are expanded on demand so any call `a(n)` with n ≥ 1 is
   answered correctly regardless of call order. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* --- static tables ---------------------------------------------------- */
    static uint64_t *primes      = NULL;
    static size_t  prime_cap    = 0;
    static size_t  prime_count  = 0;   /* number of primes stored          */

    static idx_t *good_idx      = NULL;  /* index of primes ≡ 2 mod 3 (1‑based) */
    static size_t  good_cap     = 0;
    static size_t  good_count   = 0;      /* how many good indices we have    */

    static uint64_t last_checked = 1;     /* last integer that has been tested */

    /* --- helper: enlarge an array --------------------------------------- */
    if (prime_cap == 0) {                /* initial allocation */
        prime_cap   = 16;
        primes      = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
    }
    if (good_cap == 0) {
        good_cap    = 16;
        good_idx    = (idx_t *)malloc(good_cap * sizeof(idx_t));
    }

    /* --- generate primes until we have at least `n` good indices -------- */
    while (good_count < n) {
        uint64_t cand = ++last_checked;

        /* skip even numbers > 2 */
        if (cand > 2 && (cand & 1) == 0)
            continue;

        /* test primality using the existing prime list                */
        int is_prime = 1;
        for (size_t i = 0; i < prime_count; ++i) {
            uint64_t p = primes[i];
            if (p > cand / p)                /* p² > cand */
                break;
            if (cand % p == 0) {
                is_prime = 0;
                break;
            }
        }

        if (is_prime) {
            /* store new prime ------------------------------------------------ */
            if (prime_count == prime_cap) {
                prime_cap *= 2;
                primes = (uint64_t *)realloc(primes, prime_cap * sizeof(uint64_t));
            }
            primes[prime_count++] = cand;

            /* if prime ≡ 2 mod 3, record its 1‑based index ------------------- */
            if ((cand % 3) == 2) {
                if (good_count == good_cap) {
                    good_cap *= 2;
                    good_idx = (idx_t *)realloc(good_idx, good_cap * sizeof(idx_t));
                }
                good_idx[good_count++] = (idx_t)prime_count;  /* 1‑based index of this prime */
            }
        }
    }

    /* --- answer the query ------------------------------------------------- */
    return (seq_t)good_idx[n - 1];
}
