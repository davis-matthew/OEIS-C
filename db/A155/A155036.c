/*
 * A155036  Primes p = prime(c) with index c a composite such that the
 *          concatenation c//p is a prime.
 * https://oeis.org/A155036
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   a(n) = prime(c) where c is composite and concat(c, prime(c)) is
 *            prime
 * Range:     u8 n <= 2, u16 n <= 11, u32 n <= 15, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   concat_parts, get_prime, is_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "../utils/is_prime.h"          /* bool is_prime(idx_t n) */
#include "../utils/is_prime_big.h"      /* bool is_prime_big(seq_t n) */
#include "../utils/concat_parts.h"      /* seq_t concat_parts(idx_t a, idx_t b) */
#include "../utils/get_prime.h"         /* uint32_t get_prime(idx_t k) */

/* --------- static data for memoisation --------- */
static seq_t *seq_terms   = NULL;   /* array of computed terms */
static size_t terms_len   = 0;      /* number of terms stored */
static size_t terms_cap   = 0;      /* current capacity of seq_terms */

static idx_t last_checked_c = 0;    /* last composite index examined */

/* enlarge seq_terms array if needed */
static void ensure_terms_cap(size_t needed)
{
    if (terms_cap >= needed)
        return;
    size_t new_cap = (terms_cap == 0) ? 8 : terms_cap * 2;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_terms = (seq_t *)realloc(seq_terms, new_cap * sizeof(seq_t));
    terms_cap = new_cap;
}

/* store a new term */
static void add_term(seq_t val)
{
    ensure_terms_cap(terms_len + 1);
    seq_terms[terms_len++] = val;
}

/* extend computation until we have at least target terms */
static void compute_until(idx_t target)
{
    if (terms_len >= target)
        return;

    idx_t c = last_checked_c;
    while (terms_len < target) {
        ++c;  /* next index */

        /* skip 0,1 and all prime indices */
        if (c > 1 && !is_prime(c)) {
            /* p = prime(c) */
            uint32_t p_uint32 = get_prime(c);
            seq_t p = (seq_t)p_uint32;

            /* concat(c, p) */
            seq_t concatenated = concat_parts(c, (idx_t)p_uint32);

            if (is_prime_big(concatenated)) {
                add_term(p);
            }
        }
    }

    last_checked_c = c;
}

/* ----------------------------------------------------- */
/* public sequence function required by the exercise    */
/* ----------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)  /* undefined, safety */
        return (seq_t)0;
    compute_until(n);
    return seq_terms[n - 1];
}
