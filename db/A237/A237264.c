/*
 * A237264  Number of partitions of 3n into 3 parts with largest part prime.
 * https://oeis.org/A237264
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of 3n into 3 positive parts with the
 *            largest part prime. The parts are unordered, i.e. a ≥ b ≥ c ≥ 1
 * Range:     u8 n <= 38, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses a sieve to generate primes up to the largest
   needed value (3n-2).  The result is computed directly from the
   combinatorial formula described below. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Global prime cache */
static idx_t *prime_list = NULL;   /* array of primes */
static size_t  prime_count = 0;    /* number of stored primes */
static idx_t   prime_max   = 0;    /* highest number for which primes are prepared */

/* Ensure we have all primes ≤ limit in prime_list. */
static void
ensure_primes(idx_t limit)
{
    if (limit <= prime_max)
        return;                        /* already prepared */

    /* Allocate a new sieve array */
    bool *is_composite = (bool*)calloc((size_t)(limit + 1), sizeof(bool));
    if (!is_composite)
        exit(1);                       /* allocation failed */

    /* Allocate a prime list with rough capacity.  It will grow if needed. */
    size_t cap = limit / 10 + 10;
    idx_t *new_list = (idx_t*)malloc(cap * sizeof(idx_t));
    if (!new_list)
        exit(1);

    size_t cnt = 0;
    for (idx_t p = 2; p <= limit; ++p) {
        if (!is_composite[p]) {
            /* store prime */
            if (cnt == cap) {
                cap *= 2;
                new_list = (idx_t*)realloc(new_list, cap * sizeof(idx_t));
                if (!new_list)
                    exit(1);
            }
            new_list[cnt++] = p;

            /* mark multiples */
            if (p * p <= limit) {
                for (idx_t j = p * p; j <= limit; j += p)
                    is_composite[j] = true;
            }
        }
    }

    free(is_composite);

    /* Replace the old list. */
    free(prime_list);
    prime_list = new_list;
    prime_count = cnt;
    prime_max = limit;
}

/* Main sequence function. */
seq_t
a(idx_t n)
{
    if (n == 0)
        return 0;                     /* value is undefined for n=0, return 0 */

    const idx_t three_n   = 3 * n;
    const idx_t prime_lim = three_n >= 2 ? three_n - 2 : 0;   /* largest possible prime part */

    ensure_primes(prime_lim);

    seq_t result = 0;

    for (size_t i = 0; i < prime_count; ++i) {
        idx_t p = prime_list[i];
        if (p > prime_lim)
            break;
        if (p < n)                /* largest part cannot be smaller than n */
            continue;

        const idx_t S = three_n - p;          /* sum of the remaining two parts */

        /* b ranges from ceil(S/2) to min(p, S-1) */
        const idx_t lower = (S + 1) / 2;       /* ceil(S/2) */
        const idx_t upper = (S - 1 < p) ? (S - 1) : p;   /* min(p, S-1) */

        if (lower <= upper) {
            result += (seq_t)(upper - lower + 1);
        }
    }

    return result;
}
