/*
 * A123083  Prime quartets of form k, k+6, k+12, k+18.
 * https://oeis.org/A123083
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 220, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Method: Generate all prime quartets k, k+6, k+12, k+18 by sieving
   a range of integers and append the four primes of each quartet
   to a global list. The nth term of the sequence is the (n‑1)‑th
   element of that list (offset 1). */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/seq_realloc.h"  /* for seq_realloc */

static idx_t      cur_limit   = 0;          /* current sieve upper bound */
static unsigned char *is_prime = NULL;      /* primality flags   */
static idx_t      *primes    = NULL;        /* list of primes   */
static size_t      primes_cnt = 0;          /* number of primes */

static idx_t      last_checked = 0;         /* base primes processed */

/* Concatenated list of quartets (k, k+6, k+12, k+18). */
static seq_t     *terms      = NULL;        /* array   */
static size_t      terms_cnt = 0;           /* number of terms stored   */
static size_t      terms_cap = 0;           /* capacity of the array    */

/* Reallocate the terms array to have at least new_cap elements. */
static void grow_terms(size_t new_cap)
{
    if (terms_cap >= new_cap)
        return;
    size_t cap = terms_cap ? terms_cap * 2 : new_cap;
    if (cap < new_cap)
        cap = new_cap;
    seq_t *tmp = seq_realloc(terms, terms_cap, cap);
    if (tmp) {
        terms = tmp;
        terms_cap = cap;
    }
}

/* Sieve of Eratosthenes up to new_limit (inclusive) and accumulate
   quartets whose base prime is > last_checked. */
static void sieve(idx_t new_limit)
{
    /* Allocate and initialise the sieve array. */
    unsigned char *s = (unsigned char*)calloc((size_t)new_limit + 1, sizeof(unsigned char));
    if (!s)
        return;       /* allocation failed – abort silently */

    for (idx_t i = 2; i <= new_limit; ++i)
        s[i] = 1;
    for (idx_t i = 2; i * i <= new_limit; ++i) {
        if (s[i]) {
            for (idx_t j = i * i; j <= new_limit; j += i)
                s[j] = 0;
        }
    }

    /* Count primes. */
    size_t cnt = 0;
    for (idx_t i = 2; i <= new_limit; ++i)
        if (s[i]) ++cnt;

    idx_t *p = (idx_t*)malloc(cnt * sizeof(idx_t));
    if (!p) {
        free(s);
        return;
    }

    size_t pi = 0;
    for (idx_t i = 2; i <= new_limit; ++i)
        if (s[i]) p[pi++] = i;

    /* Generate quartets for new primes. */
    size_t start = 0;
    while (start < cnt && p[start] <= last_checked)
        ++start;

    for (size_t k = start; k < cnt; ++k) {
        idx_t base = p[k];
        if (base + 18 > new_limit)
            break;               /* further primes cannot form a full quartet */
        if (s[base + 6] && s[base + 12] && s[base + 18]) {
            grow_terms(terms_cnt + 4);
            terms[terms_cnt++] = (seq_t)base;
            terms[terms_cnt++] = (seq_t)(base + 6);
            terms[terms_cnt++] = (seq_t)(base + 12);
            terms[terms_cnt++] = (seq_t)(base + 18);
        }
    }

    /* Update global state. */
    last_checked = new_limit - 18;
    if (is_prime) free(is_prime);
    if (primes)   free(primes);
    is_prime   = s;
    primes     = p;
    primes_cnt = cnt;
    cur_limit  = new_limit;
}

/* Main sequence function ------------------------------------------------ */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined: offset is 1, but safe fallback */

    /* Expand the sieve and the terms array until we have at least n terms. */
    while (terms_cnt < (size_t)n) {
        idx_t new_limit = cur_limit ? cur_limit * 2u : 100000u;
        sieve(new_limit);
    }
    return terms[n - 1];
}
