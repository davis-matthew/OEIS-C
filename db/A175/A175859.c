/*
 * A175859  a(n) = numbers k in increasing order such that A175856(m) = k has
 *          no solution for any m, where A175856(m): a(m) = m for m =
 *          noncomposites, a(m) = previous term - 1 for m = composites.
 * https://oeis.org/A175859
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 9670, u64 n <= 9670,
 *            u128 n <= 9670, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: find the n‑th missing value of sequence A175856. 
   The function iterates over increasing limits, performs a sieve,
   builds the A175856 sequence, flags the values that appear,
   and counts the numbers that never appear.
   It stops when the n‑th missing number has been found. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Function definition required by the problem statement. */
extern seq_t a(idx_t n)
{
    /* guard against invalid index */
    if (n == 0) {
        return (seq_t)0;
    }

    /* start with a moderate limit; grow it until the n-th missing
       number is found. */
    idx_t limit = 200000;    /* initial sieve size */

    while (1) {
        /* allocate flags for prime numbers and for observed  A175856 values */
        /*  char arrays are simple POD types, so C-style casts are safe in
            both C and C++. */
        char *isPrime = (char*)malloc((limit + 1) * sizeof(char));
        char *present = (char*)malloc((limit + 1) * sizeof(char));
        if (!isPrime || !present) {
            /* allocation failure: abort – this is irrecoverable */
            abort();
        }

        /* initialise flag arrays */
        for (idx_t i = 0; i <= limit; ++i) {
            isPrime[i] = 1;
            present[i] = 0;
        }
        isPrime[0] = isPrime[1] = 0;

        /* Sieve of Eratosthenes for primes up to limit */
        for (idx_t p = 2; p * p <= limit; ++p) {
            if (isPrime[p]) {
                for (idx_t j = p * p; j <= limit; j += p) {
                    isPrime[j] = 0;
                }
            }
        }

        /* compute A175856 and mark seen values */
        seq_t prev = (seq_t)1;      /* a(1) = 1 */
        present[1] = 1;             /* 1 is seen */

        for (idx_t m = 2; m <= limit; ++m) {
            seq_t cur = isPrime[m] ? (seq_t)m : prev - (seq_t)1;
            if (cur <= (seq_t)limit) {
                present[(idx_t)cur] = 1;
            }
            prev = cur;
        }

        /* count missing values in [1, limit] */
        idx_t missing_cnt = 0;
        for (idx_t x = 1; x <= limit; ++x) {
            if (!present[x]) {
                ++missing_cnt;
                if (missing_cnt == n) {
                    /* clean up and return the result */
                    free(isPrime);
                    free(present);
                    return (seq_t)x;
                }
            }
        }

        /* not enough missing numbers yet: expand the limit and try again */
        free(isPrime);
        free(present);
        limit <<= 1;   /* double the limit */
    }
}
