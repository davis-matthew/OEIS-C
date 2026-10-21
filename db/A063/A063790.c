/*
 * A063790  a(n) is the smallest prime gap between n^2 and (n+1)^2.
 * https://oeis.org/A063790
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2000, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes: a(n) = smallest prime gap between n^2 and (n+1)^2 */

/*  Static sieve helper: ensures a boolean array of primes up to "limit"   */
#include <stdlib.h>
#include <stdbool.h>

static bool *prime_flags = NULL;   /* 0..prime_limit  */
static idx_t  prime_limit = 0;

static void _ensure_limit(idx_t limit)
{
    if (limit <= prime_limit) return;

    /* Allocate a new array, copy the old entries and initialise the rest */
    bool *new_flags = (bool *)malloc((limit + 1) * sizeof(bool));
    if (!new_flags) { /* memory exhausted – abort cleanly */
        abort();
    }
    /* copy old flags if any */
    if (prime_flags) {
        for (idx_t i = 0; i <= prime_limit; ++i)
            new_flags[i] = prime_flags[i];
        free(prime_flags);
    }
    /* initialise new entries to true (potential primes) */
    for (idx_t i = prime_limit + 1; i <= limit; ++i)
        new_flags[i] = true;

    /* mark 0 and 1 as composite */
    if (limit >= 0) new_flags[0] = false;
    if (limit >= 1) new_flags[1] = false;

    /* simple sieve of Eratosthenes up to "limit" */
    for (idx_t i = 2; i * i <= limit; ++i) {
        if (new_flags[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                new_flags[j] = false;
        }
    }

    prime_flags = new_flags;
    prime_limit = limit;
}

/*  Main sequence entry  */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* undefined, but safe */

    /* compute interval [n^2+1, (n+1)^2-1] */
    idx_t lower = n * n + 1;
    idx_t upper = (n + 1) * (n + 1) - 1;

    _ensure_limit(upper);

    seq_t min_gap = 0;   /* 0 means "no pair found yet"  */
    idx_t last_prime = 0;

    for (idx_t p = lower; p <= upper; ++p) {
        if (prime_flags[p]) {
            if (last_prime != 0) {
                seq_t gap = (seq_t)(p - last_prime);
                if (min_gap == 0 || gap < min_gap)
                    min_gap = gap;
            }
            last_prime = p;
        }
    }
    return min_gap;
}
