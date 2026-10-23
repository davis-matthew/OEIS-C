/*
 * A023539  Convolution of natural numbers with composite numbers.
 * https://oeis.org/A023539
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{i=1}^n i * c_{n-i+1}, where c_j is the j-th
 *            composite number
 * Range:     u8 n <= 7, u16 n <= 62, u32 n <= 2784, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Static cache for the list of composite numbers. */
static seq_t *comps = NULL;            /* array of composites */
static idx_t comps_len = 0;            /* number of composites stored */

/* --------------------------------------------------------------------- */
/*  Ensure that at least n composite numbers are stored in `comps`.       */
/* --------------------------------------------------------------------- */
static void ensure_composites(idx_t n) {
    if (comps_len >= n)
        return;   /* already have enough */

    /* Pick a sieve limit that is guaranteed to contain at least n composites.
       For n up to 10 000 the bound 2*n+10 is more than enough. */
    idx_t limit = n * 2 + 10;

    while (1) {
        /* Allocate an array of bools.  Explicit cast needed for C++. */
        bool *is_prime = (bool*)calloc((size_t)limit + 1, sizeof(bool));
        if (!is_prime)
            abort();

        /* Mark primes. */
        for (size_t i = 2; i <= (size_t)limit; ++i)
            is_prime[i] = true;

        for (size_t p = 2; p * p <= (size_t)limit; ++p) {
            if (is_prime[p]) {
                for (size_t q = p * p; q <= (size_t)limit; q += p)
                    is_prime[q] = false;
            }
        }

        /* Count composites and build the list. */
        idx_t count = 0;
        for (size_t i = 4; i <= (size_t)limit; ++i)
            if (!is_prime[i])
                ++count;

        if (count >= n) {
            /* Allocate array of composites. */
            seq_t *new_comps = (seq_t*)malloc((size_t)count * sizeof(seq_t));
            if (!new_comps) {
                free(is_prime);
                abort();
            }

            idx_t idx = 0;
            for (size_t i = 4; i <= (size_t)limit; ++i)
                if (!is_prime[i])
                    new_comps[idx++] = (seq_t)i;   /* cast to seq_t */

            free(is_prime);
            if (comps) free(comps);
            comps      = new_comps;
            comps_len  = count;
            return;
        }

        /* Not enough composites – increase the sieve bound and try again. */
        free(is_prime);
        limit *= 2;
    }
}

/* --------------------------------------------------------------------- */
/*  Sequence function.                                                    */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n) {
    if (n == 0)
        return 0;   /* offset is 1; 0 is not defined */

    ensure_composites(n);

    seq_t sum = 0;
    for (idx_t j = 0; j < n; ++j) {
        seq_t c   = comps[j];             /* j+1-th composite */
        seq_t mul = (seq_t)(n - j);       /* (n-j) = n - (j+1) + 1 */
        sum += mul * c;
    }
    return sum;
}
