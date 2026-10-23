/*
 * A103523  Concatenations of pairs of primes that differ by 100.
 * https://oeis.org/A103523
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 5, u32 n <= 828, u64 n <= 9833, u128 n <= 9833,
 *            big any n
 * Helpers:   uint64_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes sequence A103523 (concatenations of prime pairs differing by 100).
   a(n) is the nth such concatenated integer, offset 1.  The function
   generates all pairs up to a fixed sieve limit and stores the results
   in a static array.  The array is expanded if a call requests a larger
   index than has been computed so far. */

/* Standard headers that are always allowed. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/uint64_sqrt.h"

/* ----------------------------------------------------------------------
   Static state used by the routine.
   ---------------------------------------------------------------------- */
static seq_t  *terms   = NULL;  /* array of sequence values               */
static idx_t   terms_len = 0;   /* number of terms stored in terms[]      */
static idx_t   terms_cap = 0;   /* current capacity of the array          */

/* Pre‑computed powers of ten needed for concatenation.  10^0 … 10^20 is
   more than enough for the primes we sieve (largest prime < 50 million). */
static seq_t pow10[21];

/* Sieve limit – large enough to produce at least 10 000 terms. */
#define SIEVE_LIMIT 50000000ULL  /* 50 million */

/* ----------------------------------------------------------------------
   Initialise the sieve and generate terms lazily.  This function is
   called from a() each time the routine is used; the static flag makes
   it run only once.  If the requested index is larger than the number
   of already generated terms, the sieve is not extended – we simply
   generate all terms up to the fixed SIEVE_LIMIT.
   ---------------------------------------------------------------------- */
static void init(void)
{
    static bool done = false;
    if (done) return;
    done = true;

    /* Pre‑compute powers of ten. */
    pow10[0] = (seq_t)1;
    for (int i = 1; i < 21; ++i)
        pow10[i] = pow10[i - 1] * (seq_t)10U;

    /* Allocate the prime sieve.  We need an extra 101 slots to test
       p+100 for primality when p == SIEVE_LIMIT. */
    size_t sieve_size = (size_t)(SIEVE_LIMIT + 101);
    unsigned char *is_prime = (unsigned char *)malloc(sieve_size);
    if (!is_prime) return;          /* allocation failed – abort initialization */

    memset(is_prime, 1, sieve_size);
    is_prime[0] = is_prime[1] = 0;

    /* Simple Eratosthenes sieve. */
    uint64_t lim = uint64_sqrt(SIEVE_LIMIT + 100);
    for (uint64_t i = 2; i <= lim; ++i) {
        if (!is_prime[i]) continue;
        for (uint64_t j = i * i; j <= SIEVE_LIMIT + 100; j += i)
            is_prime[j] = 0;
    }

    /* Allocate initial space for terms.  We grow this array as needed. */
    terms_cap = 20000;  /* initial capacity – plenty for ~10k terms */
    terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
    if (!terms) { free(is_prime); return; }

    /* Generate all concatenated prime pairs up to SIEVE_LIMIT. */
    for (uint64_t p = 3; p <= SIEVE_LIMIT; ++p) { /* start at 3: 2+100=102 not prime */
        if (!is_prime[p]) continue;
        uint64_t q = p + 100;
        if (!is_prime[q]) continue;

        /* Count decimal digits of q. */
        uint64_t tmp = q, digits = 0;
        do { ++digits; tmp /= 10; } while (tmp);

        seq_t term = (seq_t)p * pow10[digits] + (seq_t)q;

        /* Expand the array if it is full. */
        if (terms_len == terms_cap) {
            terms_cap <<= 1;  /* double capacity */
            seq_t *tmp_terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
            if (!tmp_terms) break;  /* allocation failed – stop adding */
            terms = tmp_terms;
        }
        terms[terms_len++] = term;
    }

    free(is_prime);
}

/* ----------------------------------------------------------------------
   Public function required by the build system.
   ---------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* safety for offset 0, though not used */

    init();                         /* ensure terms[] is filled once */

    if ((idx_t)n > terms_len)       /* request beyond what we computed */
        return (seq_t)0;            /* out‑of‑range request */

    return terms[(size_t)n - 1];
}
