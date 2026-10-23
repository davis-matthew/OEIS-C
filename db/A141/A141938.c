/*
 * A141938  Primes congruent to 14 mod 25.
 * https://oeis.org/A141938
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 327, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A141938 – Primes congruent to 39 (mod 50)
   (i.e. primes p such that p ≡ 14 (mod 25)).
   The function a(n) returns the n‑th such prime (a(1)=89, a(2)=139,…). */

/* required for C99/C23 and most C++ compilers */
#include <stdlib.h>   /* malloc, free, realloc */
#include <stddef.h>   /* size_t */
#include <stdbool.h>  /* bool, false, true */

/* the types seq_t and idx_t are defined by the build system */
static const idx_t INITIAL_LIMIT = 3000000u;   /* sufficient for n≈10 000 */

/* cache for the primes of the desired congruence class */
static seq_t *prime_cache     = NULL;   /* array of primes */
static idx_t  prime_count     = 0;      /* number stored in prime_cache */
static idx_t  prime_capacity  = 0;      /* current allocation */
static idx_t  cache_limit     = 0;      /* sieve upper bound used */

/* ------------------------------------------------------------------ */
/* create a list of all primes p ≤ limit with p ≡ 14 (mod 25).       */
/* ------------------------------------------------------------------ */
static void build_prime_cache(idx_t limit)
{
    /* free previous cache */
    if (prime_cache != NULL) {
        free(prime_cache);
        prime_cache = NULL;
    }
    prime_count  = 0;
    prime_capacity = 0;

    /* --- sieve of Eratosthenes (bool array) --- */
    bool *is_composite = (bool *)calloc((size_t)limit + 1, sizeof(bool));
    if (is_composite == NULL) {
        /* allocation failure – leave cache empty */
        return;
    }
    /* mark 0 and 1 as composite */
    if (limit >= 1) is_composite[0] = is_composite[1] = true;

    for (idx_t i = 2; i <= limit / i; ++i) {
        if (!is_composite[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                is_composite[j] = true;
        }
    }

    /* estimate number of qualifying primes to pre‑allocate */
    size_t estimate = (size_t)(limit / 20u) + 50;
    prime_cache  = (seq_t *)malloc(estimate * sizeof(seq_t));
    if (prime_cache == NULL) {
        free(is_composite);
        return;
    }
    prime_capacity = estimate;

    /* collect the primes ≡14 (mod 25) */
    for (idx_t p = 2; p <= limit; ++p) {
        if (!is_composite[p] && (p % 25u == 14u)) {
            if (prime_count == prime_capacity) {
                prime_capacity *= 2;
                seq_t *tmp = (seq_t *)realloc(prime_cache,
                                              prime_capacity * sizeof(seq_t));
                if (tmp == NULL) {
                    /* allocation failure – stop collecting */
                    break;
                }
                prime_cache = tmp;
            }
            prime_cache[prime_count++] = (seq_t)p;
        }
    }

    free(is_composite);
}

/* ------------------------------------------------------------------ */
/* guarantee that prime_cache holds at least n primes                  */
/* ------------------------------------------------------------------ */
static void ensure_cache_idx(idx_t n)
{
    if (prime_cache != NULL && n <= prime_count)
        return;            /* already enough */

    idx_t limit = cache_limit ? cache_limit : INITIAL_LIMIT;
    while (1) {
        build_prime_cache(limit);
        if (prime_cache != NULL && n <= prime_count) {
            cache_limit = limit;
            return;            /* cache ready */
        }
        /* not enough – double the limit and retry */
        limit *= 2u;
    }
}

/* ------------------------------------------------------------------ */
/* public sequence function                                         */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                    /* behaviour below offset is irrelevant */

    ensure_cache_idx(n);
    /* n is guaranteed to be ≤ prime_count */
    return prime_cache[n - 1];
}
