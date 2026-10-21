/*
 * A066533  Erroneous version of A058986.
 * https://oeis.org/A066533
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = (n - 1) + number of primes p ≤ n whose index in the prime
 *            list is itself prime. The sequence is
 *            0,1,3,4,6,7,8,9,10,11,13,14,15,... A(1)=0, a(2)=1, …
 * Range:     i8 n <= 13, i16 n <= 13, i32 n <= 13, i64 n <= 13, i128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* static state for the sieve and cumulative counts */
static bool   *is_prime   = NULL;
static int    *prime_idx  = NULL;
static int    *cum_special = NULL;
static size_t   limit     = 0;

/* Ensure that the sieve, prime indices and cumulative special prime counts
   are computed up to at least “n” (inclusive).  This function is called
   lazily by a() and grows its tables only when needed. */
static void ensure_up_to(idx_t n)
{
    if (n <= limit)
        return;

    size_t new_limit = (size_t)n + 1;                /* array indices 0…new_limit */
    bool   *new_flag = (bool   *)calloc(new_limit + 1, sizeof(bool));
    int    *new_idx  = (int    *)calloc(new_limit + 1, sizeof(int));
    int    *new_cum  = (int    *)calloc(new_limit + 1, sizeof(int));

    if (!new_flag || !new_idx || !new_cum) {
        free(new_flag); free(new_idx); free(new_cum);
        exit(1);                     /* memory allocation failed */
    }

    /* --- Eratosthenes sieve on [0,new_limit] --- */
    for (size_t i = 0; i <= new_limit; ++i)
        new_flag[i] = true;
    if (new_limit >= 0) new_flag[0] = false;
    if (new_limit >= 1) new_flag[1] = false;

    for (size_t p = 2; p * p <= new_limit; ++p) {
        if (!new_flag[p]) continue;
        for (size_t m = p * p; m <= new_limit; m += p)
            new_flag[m] = false;
    }

    /* --- Build prime indices and cumulative special prime count --- */
    int pcount = 0;
    for (size_t i = 2; i <= new_limit; ++i) {
        if (new_flag[i]) {
            ++pcount;
            new_idx[i] = pcount;            /* 1‑based index of prime i */
        } else
            new_idx[i] = 0;
    }

    int cum = 0;
    for (size_t i = 2; i <= new_limit; ++i) {
        /* A "special" prime is prime i whose index is also prime
           (using the same is_prime array which is valid up to new_limit). */
        if (new_flag[i] && new_flag[new_idx[i]])
            ++cum;
        new_cum[i] = cum;
    }
    /* cum_special[0] and cum_special[1] remain 0 from calloc */

    /* Replace old tables with the new ones */
    free(is_prime);   free(prime_idx);  free(cum_special);
    is_prime   = new_flag;
    prime_idx  = new_idx;
    cum_special = new_cum;
    limit = new_limit;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset is 1; outside range arbitrary */
    ensure_up_to(n);
    return (seq_t)(n - 1) + (seq_t)cum_special[n];
}
