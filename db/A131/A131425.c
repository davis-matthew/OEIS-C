/*
 * A131425  Row sums of triangle A131424.
 * https://oeis.org/A131425
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(prime(n) - 3) + Sum_{k=1..n} prime(k)
 * Range:     u8 n <= 8, u16 n <= 92, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

/* prime generation and dynamic array utilities */
#include "../utils/build_primes_upto.h"   /* build_primes_upto */
#include "../utils/seq_realloc.h"         /* seq_realloc */

/* static state */
static uint64_t *primes = NULL;       /* array of primes (0‑based) */
static size_t   prime_count = 0;      /* number of primes stored */
static idx_t    prime_limit = 0;      /* sieve limit used */

static seq_t *prime_prefix = NULL;    /* prefix sums of primes */
static size_t prefix_len = 0;         /* number of prefix sums stored */

/* ------------------------------------------------------------------ */
/*  Estimate an upper bound for the nth prime (n >= 6).                 */
/*  (For n < 6 we return a constant high enough to cover the first 5). */
static idx_t
prime_upper_bound(idx_t n)
{
    if (n < 6) return 15;             /* 5th prime is 11 */
    double nn = (double)n;
    double bound = nn * (log(nn) + log(log(nn)));
    return (idx_t)(bound + 3.0);     /* safety margin */
}

/* ------------------------------------------------------------------ */
/*  Grow the prime table so that it contains at least 'n' primes.      */
/*  The table may be rebuilt with a larger sieve limit if necessary.  */
static void
ensure_primes(idx_t n)
{
    if (prime_count >= (size_t)n) return;

    /* Compute a suitable sieve limit until we have enough primes. */
    idx_t limit = prime_upper_bound(n);
    while (true) {
        uint64_t *new_primes = NULL;
        size_t   new_count  = 0;
        build_primes_upto(limit, &new_primes, &new_count);

        /* Replace old prime array. */
        free(primes);
        primes = new_primes;
        prime_count = new_count;
        prime_limit = limit;

        /* Ensure that the prefix‑sum array matches the new prime list. */
        /* Reallocate prefix array to the new size. */
        prime_prefix = seq_realloc(prime_prefix, prefix_len, prime_count);
        if (!prime_prefix) abort();

        /* Recompute all prefix sums. */
        for (size_t i = 0; i < prime_count; ++i) {
            if (i == 0)
                prime_prefix[i] = (seq_t)primes[0];
            else
                prime_prefix[i] = prime_prefix[i - 1] + (seq_t)primes[i];
        }
        prefix_len = prime_count;

        if (prime_count >= (size_t)n) return;

        /* If still not enough primes, double the limit and try again. */
        limit *= 2;
    }
}

/* ------------------------------------------------------------------ */
/*  OEIS sequence A131425.  External linkage, as required.            */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* safety for invalid indices */

    /* Make sure we have at least n primes. */
    ensure_primes(n);

    /* Sum of the first n primes. */
    seq_t sum_primes = prime_prefix[n - 1];

    /* Term: n * (prime(n) - 3) + sum_primes */
    seq_t term = ((seq_t)n) * (((seq_t)primes[n - 1]) - (seq_t)3) + sum_primes;
    return term;
}
