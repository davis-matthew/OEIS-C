/*
 * A087683  Numbers n such that n + 10 and n - 10 are both prime.
 * https://oeis.org/A087683
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function implements OEIS A087683:
   a(n) is the nth integer n such that n−10 and n+10 are both prime.
   The implementation builds a prime sieve up to a dynamic limit
   and collects all mid‑points of prime pairs with distance 20.  */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* static state for the sieve and the sequence terms */
static size_t sieve_limit = 0;         /* highest number sieved so far */
static uint8_t *prime_flags = NULL;    /* sieve bit array (0 = composite, 1 = prime) */

static size_t terms_len   = 0;         /* number of terms currently stored */
static size_t terms_cap   = 0;         /* capacity of the terms array */
static seq_t *terms       = NULL;     /* array of sequence terms */

/* ------------------------------------------------------------------ */
/* tiny helper: create/resize a boolean array for the sieve            */
/* ------------------------------------------------------------------ */
static void init_sieve(size_t limit)
{
    /* free the old sieve if it exists */
    if (prime_flags) {
        free(prime_flags);
    }

    /* allocate and initialise to 1 (prime) */
    prime_flags = (uint8_t *)calloc(limit + 1, sizeof(uint8_t));
    if (!prime_flags) return; /* allocation failed – leave in a bad state */

    for (size_t i = 2; i <= limit; ++i) {
        prime_flags[i] = 1;
    }

    /* sieve out composites */
    for (size_t i = 2; (i * i) <= limit; ++i) {
        if (prime_flags[i]) {
            for (size_t j = i * i; j <= limit; j += i) {
                prime_flags[j] = 0;
            }
        }
    }

    sieve_limit = limit;
}

/* ------------------------------------------------------------------ */
/* rebuild the list of sequence terms from the current sieve           */
/* ------------------------------------------------------------------ */
static void rebuild_terms(void)
{
    /* free the old terms array if it exists */
    if (terms) {
        free(terms);
    }

    /* rough estimate of how many pairs we'll get – avoid allocating zero */
    terms_cap = sieve_limit / 10;
    if (terms_cap < 100) terms_cap = 100;

    terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
    if (!terms) {
        terms_len = 0;
        return;
    }

    terms_len = 0;

    /* scan for prime pairs with distance 20 */
    for (size_t p = 2; p + 20 <= sieve_limit; ++p) {
        if (prime_flags[p] && prime_flags[p + 20]) {
            seq_t val = (seq_t)(p + 10);   /* middle of the pair */

            if (terms_len == terms_cap) {
                terms_cap <<= 1;           /* grow capacity */
                seq_t *tmp = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
                if (!tmp) {                 /* realloc failed – abort */
                    free(terms);
                    terms = NULL;
                    terms_len = 0;
                    return;
                }
                terms = tmp;
            }

            terms[terms_len++] = val;
        }
    }
}

/* ------------------------------------------------------------------ */
/* ensure we have at least 'needed' terms pre‑computed                  */
/* ------------------------------------------------------------------ */
static void ensure_terms(idx_t needed)
{
    if ((size_t)terms_len >= (size_t)needed) return;

    /* grow the sieve until enough terms are available */
    size_t new_limit = (sieve_limit > 0) ? sieve_limit : 100000; /* start guess */
    while ((size_t)terms_len < (size_t)needed) {
        new_limit <<= 1;          /* double the search space each time */
        init_sieve(new_limit);
        rebuild_terms();
    }
}

/* ------------------------------------------------------------------ */
/* public entry point                                                     */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* outside the offset – not used */

    static bool initialized = false;
    if (!initialized) {
        init_sieve(10000020);      /* initial bound: 10 000 020 */
        rebuild_terms();
        initialized = true;
    }

    ensure_terms(n);
    return terms[n - 1];          /* sequence is 1‑based */
}
