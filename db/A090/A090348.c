/*
 * A090348  Norms of prime elements in the ring of integers of Q(sqrt(-7)).
 * https://oeis.org/A090348
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Norms of prime elements in Q(sqrt(-7)).
   The sequence is obtained by merging the list of split primes
   (p = 2, 7 or p ≡ 1,2,4 (mod 7)) and the list of inert primes
   (p ≡ 3,5,6 (mod 7)) where inert primes contribute p².
   The sequence terms are sorted in increasing order. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* --------------------------------------------------------------------
   Sieve facility – generates primes up to a compile‑time limit.
   The limit is chosen to provide at least 10 000 sequence terms
   (≈ 150 000 primes).  The sieve is executed only once
   (lazy evaluation).                                           ---- */
#define MAX_PRIME_LIMIT 2000000          /* prime bound for the sieve   */
#define MAX_PRIMES      150000           /* approximate π(MAX)         */
#define MAX_CACHE       (MAX_PRIMES * 2)  /* enough space for all terms */

/* Global storage for primes.  The arrays are only written once. */
static uint64_t primes[MAX_PRIMES];
static size_t primes_cnt = 0;

/* Stores norms for split and inert primes. */
static seq_t split_vals[MAX_PRIMES];
static size_t split_cnt = 0;

static seq_t inert_vals[MAX_PRIMES];
static size_t inert_cnt  = 0;

/* Merged cache of sequence values. */
static seq_t cache[MAX_CACHE];
static size_t cache_len = 0;

/* Flag marking whether the sieve has been performed. */
static bool sieve_done = false;

/* --------------------------------------------------------------------
   Sieve of Eratosthenes – fills the primes array.  The routine is
   called lazily when the first request for the sequence is made.
   -------------------------------------------------------------------- */
static void run_sieve(void)
{
    if (sieve_done)
        return;

    const size_t N = (size_t)MAX_PRIME_LIMIT;
    bool *is_comp = (bool *)calloc(N + 1, sizeof(bool));
    if (!is_comp)
        abort();

    size_t count = 0;
    for (size_t i = 2; i <= N; ++i) {
        if (!is_comp[i]) {
            if (count < MAX_PRIMES) {
                primes[count++] = (uint64_t)i;
            }
            for (size_t j = i * 2; j <= N; j += i)
                is_comp[j] = true;
        }
    }
    primes_cnt = count;
    free(is_comp);

    /* Split the primes into the two categories. */
    for (size_t idx = 0; idx < primes_cnt; ++idx) {
        uint64_t p = primes[idx];
        if (p == 2 || p == 7 ||
            p % 7 == 1 || p % 7 == 2 || p % 7 == 4) {
            if (split_cnt < MAX_PRIMES)
                split_vals[split_cnt++] = (seq_t)p;
        } else { /* inert */
            if (inert_cnt < MAX_PRIMES)
                inert_vals[inert_cnt++] = (seq_t)p * (seq_t)p;
        }
    }

    /* Merge the two monotone lists into cache. */
    size_t iS = 0, iI = 0, ic = 0;
    while ((iS < split_cnt) || (iI < inert_cnt)) {
        seq_t next;
        if (iS < split_cnt) {
            if (iI < inert_cnt) {
                if (split_vals[iS] <= inert_vals[iI]) {
                    next = split_vals[iS++];
                } else {
                    next = inert_vals[iI++];
                }
            } else { /* only split left */
                next = split_vals[iS++];
            }
        } else { /* only inert left */
            next = inert_vals[iI++];
        }
        if (ic < MAX_CACHE)
            cache[ic++] = next;
    }
    cache_len = ic;

    sieve_done = true;
}

/* --------------------------------------------------------------------
   a(idx_t n) – public entry point.
   -------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* the sequence is 1‑based; n=0 is an invalid request: */
    if (n == 0)
        return (seq_t)0;

    run_sieve();                 /* ensure primes and cache are ready */

    if ((idx_t)n <= cache_len)
        return cache[(size_t)n - 1];

    /* In practice with the chosen limits this branch is never taken
       for the intended range (n up to about 10 000).  If it is, we
       do a linear search on the fly to produce the missing terms.  */
    size_t iS = 0, iI = 0;
    size_t terms_seen = 1; /* we already know cache[0] exists */

    while (terms_seen < n) {
        seq_t next;
        if (iS < split_cnt) {
            if (iI < inert_cnt) {
                if (split_vals[iS] <= inert_vals[iI]) {
                    next = split_vals[iS++];
                } else {
                    next = inert_vals[iI++];
                }
            } else {
                next = split_vals[iS++];
            }
        } else {
            next = inert_vals[iI++];
        }
        if (terms_seen + 1 == n)
            return next;
        ++terms_seen;
    }

    /* fallback – should never happen */
    return (seq_t)0;
}
