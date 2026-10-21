/*
 * A072966  Numbers which are not the sum of two semiprimes.
 * https://oeis.org/A072966
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 13, u32 n <= 13, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A072966 – numbers that are not the sum of two semiprimes.
   The function builds a list of semiprimes up to the largest integer
   examined and tests each candidate integer for representability as
   a sum of two semiprimes.  Results are memoised in a static array,
   allowing calls in any order.  No hard‑coded table of values is used. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

static idx_t  check_limit   = 0;          /* maximum number prepared so far */
static uint64_t *spf        = NULL;       /* smallest prime factor table */
static uint8_t  *semiprime_flag = NULL;   /* flag: 1 if index is semiprime */
static uint64_t *semiprimes  = NULL;      /* list of semiprimes */
static size_t    semiprimes_len  = 0;
static size_t    semiprimes_cap  = 0;

/* static storage for the sequence values (as uint64_t) */
static uint64_t *terms      = NULL;     /* terms array (0‑based index)  */
static size_t   terms_len    = 0;
static size_t   terms_cap    = 0;

/* last integer that has been examined for representation */
static idx_t   last_checked = 0;

/* --------------------------------------------------------------------- */
/* Expand tables so that all numbers up to 'up_to' (inclusive) are ready. */
static void ensure_limit(idx_t up_to)
{
    if (up_to <= check_limit) return;

    uint64_t N = (uint64_t)up_to;

    /* free any existing tables */
    free((void*)spf);
    free((void*)semiprime_flag);
    free((void*)semiprimes);

    /* allocate new tables */
    spf            = (uint64_t *)calloc(N + 1, sizeof(uint64_t));
    semiprime_flag = (uint8_t  *)calloc(N + 1, sizeof(uint8_t ));
    if (!spf || !semiprime_flag) exit(EXIT_FAILURE);

    /* initialise spf array with identity */
    for (uint64_t i = 0; i <= N; ++i) spf[i] = i;

    /* sieve for smallest prime factor */
    for (uint64_t i = 2; i <= N / i; ++i) { /* i*i <= N */
        if (spf[i] == i) {                     /* i is prime */
            for (uint64_t j = i * i; j <= N; j += i)
                if (spf[j] == j) spf[j] = i;
        }
    }

    /* reserve a rough capacity for semiprimes */
    semiprimes_cap = N / 10 + 16;  /* rough estimate, grows on demand */
    semiprimes = (uint64_t *)malloc(semiprimes_cap * sizeof(uint64_t));
    if (!semiprimes) exit(EXIT_FAILURE);
    semiprimes_len = 0;

    /* build flag array and list */
    for (uint64_t n = 4; n <= N; ++n) {
        uint64_t p = spf[n];
        uint64_t q = n / p;
        if (q > 1 && spf[q] == q) {                  /* both primes */
            semiprime_flag[(size_t)n] = 1;
            if (semiprimes_len == semiprimes_cap) {
                semiprimes_cap <<= 1;                 /* double */
                uint64_t *tmp = (uint64_t *)realloc(semiprimes,
                                                     semiprimes_cap * sizeof(uint64_t));
                if (!tmp) exit(EXIT_FAILURE);
                semiprimes = tmp;
            }
            semiprimes[semiprimes_len++] = n;
        }
    }
    check_limit = up_to;
}

/* --------------------------------------------------------------------- */
/* Return the n‑th term of A072966. (1‑based indexing.) */
seq_t a(idx_t n)
{
    /* initialise on first invocation */
    if (terms_len == 0) {
        terms_cap = 64;
        terms = (uint64_t *)calloc(terms_cap, sizeof(uint64_t));
        if (!terms) exit(EXIT_FAILURE);
        terms[terms_len++] = 0;        /* a(1) = 0 */
        last_checked = 0;
    }

    /* generate terms until we have at least n entries */
    while (terms_len < (size_t)n) {
        idx_t m = last_checked + 1;          /* next integer to test */
        ensure_limit(m);

        bool representable = false;
        /* check if m can be expressed as sum of two semiprimes */
        for (size_t i = 0; i < semiprimes_len; ++i) {
            uint64_t s = semiprimes[i];
            if (s > (uint64_t)m) break;
            uint64_t rem = (uint64_t)m - s;
            if (rem <= check_limit && semiprime_flag[(size_t)rem]) {
                representable = true;
                break;
            }
        }

        if (!representable) {
            /* store new term */
            if (terms_len == terms_cap) {
                terms_cap <<= 1;
                uint64_t *tmp = (uint64_t *)realloc(terms,
                                                    terms_cap * sizeof(uint64_t));
                if (!tmp) exit(EXIT_FAILURE);
                terms = tmp;
            }
            terms[terms_len++] = (uint64_t)m;
        }

        last_checked = m;
    }

    return (seq_t)terms[n - 1];
}
