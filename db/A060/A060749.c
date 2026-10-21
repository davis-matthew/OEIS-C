/*
 * A060749  Triangle in which n-th row lists all primitive roots modulo the
 *          n-th prime.
 * https://oeis.org/A060749
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, nice, easy
 * Formula:   not stated
 * Range:     u8 n <= 2518, u16 n <= 9076, u32 n <= 9076, u64 n <= 9076,
 *            u128 n <= 9076, big any n
 * Helpers:   distinct_factors, find_primitive_root, powmod_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: flattened triangle of primitive roots modulo the n-th prime */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include "../utils/distinct_factors.h"
#include "../utils/find_primitive_root.h"
#include "../utils/powmod_idx.h"

/* --------------------------------------------------------------------- */
/* Types and utility functions                                         */
/* --------------------------------------------------------------------- */

typedef uint64_t idx_t;            /* index type (unsigned) */

static idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

static int cmp_idx(const void *a, const void *b)
{
    idx_t va = *(const idx_t *)a;
    idx_t vb = *(const idx_t *)b;
    return (va < vb) ? -1 : (va > vb) ? 1 : 0;
}

/* --------------------------------------------------------------------- */
/* Dynamic arrays for primes and the term sequence                       */
/* --------------------------------------------------------------------- */

static idx_t *prime_list = NULL;
static size_t prime_len  = 0, prime_cap  = 0;

static seq_t *term_list = NULL;
static size_t term_len  = 0, term_cap  = 0;

static void ensure_prime_cap(size_t new_cap)
{
    if (prime_cap < new_cap) {
        size_t nc = prime_cap ? prime_cap << 1 : 8;
        if (nc < new_cap) nc = new_cap;
        idx_t *tmp = (idx_t *)realloc(prime_list, nc * sizeof(idx_t));
        if (!tmp) exit(1);
        prime_list = tmp;
        prime_cap  = nc;
    }
}

static void ensure_term_cap(size_t new_cap)
{
    if (term_cap < new_cap) {
        size_t nc = term_cap ? term_cap << 1 : 8;
        if (nc < new_cap) nc = new_cap;
        seq_t *tmp = (seq_t *)realloc(term_list, nc * sizeof(seq_t));
        if (!tmp) exit(1);
        term_list = tmp;
        term_cap  = nc;
    }
}

/* --------------------------------------------------------------------- */
/* Prime generation                                                      */
/* --------------------------------------------------------------------- */

static idx_t last_prime = 0;   /* last prime produced */

static idx_t next_prime(void)
{
    if (last_prime < 2) {
        last_prime = 2;
        ensure_prime_cap(1);
        prime_list[prime_len++] = last_prime;
        return last_prime;
    }

    idx_t cand = last_prime + 1;
    if (cand % 2 == 0) cand++;

    for (;;) {
        idx_t limit = (idx_t)sqrt((double)cand);
        int is_pr = 1;
        for (size_t i = 0; i < prime_len && prime_list[i] <= limit; ++i) {
            if (cand % prime_list[i] == 0) {
                is_pr = 0;
                break;
            }
        }
        if (is_pr) {
            last_prime = cand;
            ensure_prime_cap(prime_len + 1);
            prime_list[prime_len++] = last_prime;
            return last_prime;
        }
        cand += 2;
    }
}

/* --------------------------------------------------------------------- */
/* Primitive roots for a single odd prime                               */
/* --------------------------------------------------------------------- */

static void generate_primitive_roots_p(idx_t p)
{
    if (p == 2) {
        ensure_term_cap(term_len + 1);
        term_list[term_len++] = (seq_t)1;
        return;
    }

    idx_t factors[32];
    size_t fcnt;
    distinct_factors(p - 1, factors, &fcnt);

    idx_t g0 = find_primitive_root(p, factors, fcnt);

    idx_t *roots = NULL;
    size_t roots_len = 0, roots_cap = 0;
    idx_t val = g0 % p;          /* g0^1 */
    for (idx_t k = 1; k < p; ++k) {
        if (gcd_idx(k, p - 1) == 1) {
            if (roots_len == roots_cap) {
                size_t nc = roots_cap ? roots_cap << 1 : 16;
                idx_t *tmp = (idx_t *)realloc(roots, nc * sizeof(idx_t));
                if (!tmp) exit(1);
                roots = tmp;
                roots_cap = nc;
            }
            roots[roots_len++] = val;
        }
        val = (idx_t)((unsigned __int128)val * g0 % p);
    }

    qsort(roots, roots_len, sizeof(idx_t), cmp_idx);

    ensure_term_cap(term_len + roots_len);
    for (size_t i = 0; i < roots_len; ++i)
        term_list[term_len++] = (seq_t)roots[i];

    free(roots);
}

/* --------------------------------------------------------------------- */
/* Main function requested by the problem                                */
/* --------------------------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* out of range safety */

    while (term_len < n) {
        idx_t prime = next_prime();
        generate_primitive_roots_p(prime);
    }
    return term_list[n - 1];
}
