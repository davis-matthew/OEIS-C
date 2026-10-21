/*
 * A073679  a(1) = 4, a(n+1) is the smallest composite number > a(n) such that
 *          all of the differences a(n+1)-a(n) are distinct primes.
 * https://oeis.org/A073679
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=4; for n≥1,
   a(n)=a(n-1)+p where p is the smallest unused prime such that
   a(n-1)+p is composite.  The differences are distinct primes. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/isPrimeGeneric.h"   /* isPrimeGeneric(seq_t) */

/* static state ----------------------------------------------------------- */
static bool initialized = false;
static size_t primes_cnt = 0;
static uint32_t *prime_arr = NULL;   /* list of primes up to the chosen limit */
static bool *used_prime = NULL;      /* prime already used as a difference */

static size_t seq_len = 0;
static size_t seq_cap = 0;
static seq_t *seq_arr = NULL;        /* memoised a(n) values */

/* ----------------------------------------------------------------------- */
static void init(void)
{
    if (initialized) return;

    /* ------------------------------------------------- */
    /* generate all primes up to a fixed limit
       200000 is more than enough for the first 10000 distinct primes           */
    const int limit = 200000;
    bool *sieve = (bool *)calloc(limit + 1, sizeof(bool));
    for (int i = 2; i <= limit; ++i) sieve[i] = true;
    for (int p = 2; p * p <= limit; ++p)
        if (sieve[p])
            for (int j = p * p; j <= limit; j += p)
                sieve[j] = false;

    /* count primes */
    for (int i = 2; i <= limit; ++i)
        if (sieve[i]) ++primes_cnt;

    prime_arr   = (uint32_t *)malloc(primes_cnt * sizeof(uint32_t));
    used_prime  = (bool *)malloc(primes_cnt * sizeof(bool));

    size_t idx = 0;
    for (int i = 2; i <= limit; ++i)
        if (sieve[i]) prime_arr[idx++] = (uint32_t)i;   /* idx must reach primes_cnt */

    for (size_t i = 0; i < primes_cnt; ++i) used_prime[i] = false;
    free(sieve);

    /* ------------------------------------------------------------------- */
    /* initialise memoised sequence with a(0)=4 */
    seq_len = 1;
    seq_cap = 16;
    seq_arr  = (seq_t *)malloc(seq_cap * sizeof(seq_t));
    seq_arr[0] = (seq_t)4;
    initialized = true;
}

/* ----------------------------------------------------------------------- */
static void generate_next(void)
{
    seq_t cur = seq_arr[seq_len - 1];

    for (size_t i = 0; i < primes_cnt; ++i) {
        if (used_prime[i]) continue;
        seq_t cand = cur + (seq_t)prime_arr[i];
        if (!isPrimeGeneric(cand)) {                 /* candidate is composite */
            /* ensure capacity */
            if (seq_len == seq_cap) {
                seq_cap *= 2;
                seq_arr = (seq_t *)realloc(seq_arr,
                                           seq_cap * sizeof(seq_t));
            }
            seq_arr[seq_len++] = cand;
            used_prime[i] = true;
            break;
        }
    }
}

/* ----------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    init();

    /* extend memoisation if necessary */
    while (seq_len <= (size_t)n)
        generate_next();

    return seq_arr[(size_t)n];
}
