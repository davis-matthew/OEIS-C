/*
 * A157410  Final primes in successive prime centuries.
 * https://oeis.org/A157410
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest prime ≤ 100 * n
 * Range:     u8 n <= 2, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* static state for incremental sieve */
static idx_t sieve_limit = 0;      /* currently sieved up to index */
static bool *prime_flags = NULL;   /* true if index is prime */
static seq_t *last_prime = NULL;   /* largest prime ≤ i for each index */

/* extend sieve and prefix array to cover [0 .. limit] */
static void ensure_sieve(idx_t limit)
{
    if (limit <= sieve_limit)
        return;                     /* nothing to do */

    idx_t old_limit = sieve_limit;

    /* enlarge prime flag array */
    bool *new_flags = (bool *)realloc(prime_flags, sizeof(bool) * (limit + 1));
    if (!new_flags)
        abort();                    /* allocation failure */
    prime_flags = new_flags;

    /* initialise new entries as prime candidates */
    for (idx_t i = old_limit + 1; i <= limit; ++i)
        prime_flags[i] = true;

    /* ensure 0 and 1 are marked non‑prime */
    if (limit >= 0)
        prime_flags[0] = false;
    if (limit >= 1)
        prime_flags[1] = false;

    /* incremental sieve: mark composites beyond old_limit */
    for (idx_t i = 2; i <= limit / i; ++i) { /* i <= sqrt(limit) */
        if (prime_flags[i]) {
            idx_t start = (old_limit / i + 1) * i; /* first multiple > old_limit */
            if (start < i * i)
                start = i * i;                     /* ensure start >= i*i */
            for (idx_t j = start; j <= limit; j += i)
                prime_flags[j] = false;
        }
    }

    /* enlarge prefix array holding largest prime ≤ i */
    seq_t *new_last = (seq_t *)realloc(last_prime, sizeof(seq_t) * (limit + 1));
    if (!new_last)
        abort();
    last_prime = new_last;

    /* compute prefix for new indices, preserving earlier entries */
    if (old_limit == 0) {
        last_prime[0] = 0;
        last_prime[1] = 0;
    }
    for (idx_t i = old_limit + 1; i <= limit; ++i) {
        if (prime_flags[i])
            last_prime[i] = (seq_t)i;
        else
            last_prime[i] = last_prime[i - 1];
    }

    sieve_limit = limit;
}

/* OEIS sequence A157410 */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* undefined for offset 0 */

    idx_t limit = 100 * n;            /* bound for the n‑th prime century */
    ensure_sieve(limit);
    return last_prime[limit];
}
