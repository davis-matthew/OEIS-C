/*
 * A166462  Primes from twin prime pairs.
 * https://oeis.org/A166462
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) lists, in ascending order of twin‑prime pairs, the integer
 *            quotients (digit_sum(p) + digit_sum(q)) / (digit_count(p) +
 *            digit_count(q)) where p and q are twin primes (q = p + 2). Only
 *            quotients that are prime appear in the sequence
 * Range:     u8 n <= 3, u16 n <= 77, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   build_primes, digit_sum, digits, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/build_primes.h"
#include "../utils/digit_sum.h"
#include "../utils/digits.h"
#include "../utils/is_prime_small.h"

static idx_t  *primes        = NULL;  /* array of primes */
static size_t  primes_cnt    = 0;     /* number of primes */

/*  Sequence cache ----------------------------------------------------- */
static seq_t *seq_cache      = NULL;  /* computed primes from the sequence */
static size_t seq_len        = 0;     /* number of terms stored */
static size_t seq_cap        = 0;     /* capacity of seq_cache */

/*  Current prime upper bound ----------------------------------------- */
static idx_t   current_limit  = 0;     /* last generated prime bound */

/*  Helper: rebuild the prime list and sequence for a given limit  ---- */
static void rebuild_sequence(idx_t limit)
{
    /* Free previous pools if any */
    if (primes)     free(primes);
    if (seq_cache)  free(seq_cache);

    /* Generate all primes up to the limit */
    build_primes(limit, &primes, &primes_cnt);

    /* Allocate an initial cache for the sequence */
    seq_cap  = 1 << 10;                     /* 1024 */
    seq_cache = (seq_t*)malloc(seq_cap * sizeof(seq_t));
    if (!seq_cache) exit(EXIT_FAILURE);
    seq_len = 0;

    /* Scan all adjacent primes for twin‑prime pairs */
    for (size_t i = 0; i + 1 < primes_cnt; ++i) {
        if (primes[i] + 2U != primes[i + 1]) continue;   /* not a twin pair */

        seq_t sum  = digit_sum((seq_t)primes[i]) +
                     digit_sum((seq_t)primes[i + 1]);

        int dcnt = digits(primes[i]) + digits(primes[i + 1]);

        /* If the division is integral we get a quotient */
        if (sum % (seq_t)dcnt == 0) {
            seq_t q = sum / (seq_t)dcnt;
            if (is_prime_small(q)) {
                /* Grow cache if needed */
                if (seq_len == seq_cap) {
                    seq_cap <<= 1;
                    seq_cache = (seq_t*)realloc(seq_cache,
                                                seq_cap * sizeof(seq_t));
                    if (!seq_cache) exit(EXIT_FAILURE);
                }
                seq_cache[seq_len++] = q;
            }
        }
    }

    current_limit = limit;
}

/*  Main function ------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined for zero index */

    /* Build enough terms using an expanding upper bound */
    while (seq_len < n) {
        idx_t new_limit = (current_limit == 0) ? 200000ULL : current_limit << 1;
        rebuild_sequence(new_limit);
    }

    return seq_cache[n - 1];   /* 1‑based indexing */
}
