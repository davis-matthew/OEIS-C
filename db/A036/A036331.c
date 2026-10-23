/*
 * A036331  Composite numbers n such that juxtaposition of prime factors of n
 *          has length 7.
 * https://oeis.org/A036331
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A036331: Composite numbers n such that the juxtaposition of the prime
   factors of n has total digit length 7.  The function builds the list
   of all such numbers up to the last known term (9943081) and returns the
   n‑th term. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

/* Globals (static to have external linkage only for a()). */
static const idx_t SPF_LIMIT = 10000000;          /* covers a(919196) */
static uint32_t *spf = NULL;                     /* smallest prime factor */
static unsigned char *digit_len_arr = NULL;      /* digit length of primes */

static seq_t *terms = NULL;                      /* cached terms of the sequence */
static size_t terms_cap = 0;                     /* allocated capacity */
static size_t terms_len = 0;                     /* number of terms stored */

/* ------------------------------------------------------------------ */
/* Initialise smallest‑prime‑factor array and digit lengths of primes. */
static void init_tables(void)
{
    if (spf != NULL) return;    /* already initialised */

    /* Allocate arrays.  Cast to required pointer types (needed for C++). */
    spf = (uint32_t *)calloc(SPF_LIMIT + 1, sizeof(uint32_t));
    if (spf == NULL) return;    /* OOM – leave build_terms() to fail gracefully */

    digit_len_arr = (unsigned char *)malloc((SPF_LIMIT + 1) * sizeof(unsigned char));
    if (digit_len_arr == NULL) { free(spf); spf = NULL; return; }

    /* Linear sieve for smallest prime factor. */
    uint32_t *primes = (uint32_t *)malloc((SPF_LIMIT / 10 + 1) * sizeof(uint32_t));
    size_t prime_cnt = 0;
    for (uint32_t i = 2; i <= SPF_LIMIT; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            primes[prime_cnt++] = i;
        }
        for (size_t j = 0; j < prime_cnt; ++j) {
            uint32_t p = primes[j];
            uint64_t prod = (uint64_t)i * p;
            if (prod > SPF_LIMIT) break;
            spf[(uint32_t)prod] = p;
            if (p == spf[i]) break;
        }
    }
    free(primes);

    /* Pre‑compute the decimal digit length of each prime up to SPF_LIMIT. */
    for (uint32_t i = 2; i <= SPF_LIMIT; ++i) {
        if (spf[i] == i) {                 /* i is prime */
            uint32_t x = i;
            unsigned char d = 0;
            do { ++d; x /= 10; } while (x);
            digit_len_arr[i] = d;
        }
    }
}

/* ------------------------------------------------------------------ */
/* Append a term to the cached sequence, reallocating if necessary. */
static void push_term(seq_t val)
{
    if (terms_len == terms_cap) {
        size_t new_cap = terms_cap ? terms_cap << 1 : 4096;
        seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        if (tmp == NULL) return;      /* OOM – silently ignore this term */
        terms = tmp;
        terms_cap = new_cap;
    }
    terms[terms_len++] = val;
}

/* ------------------------------------------------------------------ */
/* Build the whole sequence up to SPF_LIMIT (first call only). */
static void build_terms(void)
{
    init_tables();
    if (spf == NULL) return;    /* failed initialisation */

    for (idx_t n = 4; n <= SPF_LIMIT; ++n) {
        if (spf[n] == (uint32_t)n) continue;    /* skip primes */

        idx_t tmp = n;
        int total_digits = 0;
        while (tmp > 1) {
            uint32_t p = spf[tmp];
            int exp = 0;
            while ((tmp % p) == 0) { tmp /= p; ++exp; }
            total_digits += exp * digit_len_arr[p];
            if (total_digits > 7) break;
        }
        if (total_digits == 7) {
            push_term((seq_t)n);
        }
    }
}

/* ------------------------------------------------------------------ */
/* Public interface: returns the n‑th term (1‑based indexing). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;       /* undefined – return 0 */

    if (terms_len == 0) build_terms(); /* lazy initialisation */

    if (n <= terms_len) return terms[n - 1];
    return (seq_t)0;                   /* n beyond pre‑computed range */
}
