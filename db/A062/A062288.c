/*
 * A062288  Numbers k such that prime(k)+50 is also prime.
 * https://oeis.org/A062288
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compose a(k): the k‑th term of A062288.  A term is an index i such that
 * prime(i)+50 is also prime.  The sequence is indexed from 1.  The routine
 * generates primes on demand and memoises all qualifying indices; subsequent
 * calls are answered from the cache.  The implementation uses the helper
 * function is_prime(idx_t) from "../utils/is_prime.h". */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

/* seq_t and idx_t are provided by the build system. */

/* Cached list of qualifying indices (terms of the sequence). */
static seq_t *terms = NULL;       /* array of seq_t values */
static size_t terms_cap = 0;      /* current allocated capacity */
static size_t terms_cnt = 0;      /* number of terms currently stored */

/* State used while generating primes. */
static idx_t next_candidate = 0;  /* candidate to test for primality */
static idx_t prime_index = 0;     /* number of primes encountered so far (i.e. current k) */

/* Ensure the terms array can hold at least 'need' elements. */
static void ensure_terms(size_t need)
{
    if (terms_cap < need) {
        size_t new_cap = terms_cap ? terms_cap << 1 : 8;
        while (new_cap < need)
            new_cap <<= 1;
        terms = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        terms_cap = new_cap;
    }
}

/* Generate primes and qualifying indices until at least 'n' terms are known. */
static void generate_up_to(idx_t n)
{
    if (n == (idx_t)0 || terms_cnt >= (size_t)n)
        return;

    /* Initialise the state on the first call. */
    if (next_candidate == 0) {
        next_candidate = 2;   /* start testing from the first prime */
        prime_index = 0;
    }

    while (terms_cnt < (size_t)n) {
        idx_t p;

        /* Find the next prime. */
        if (next_candidate == 2) {          /* prime 2 is handled once */
            p = 2;
            next_candidate = 3;              /* next candidate will be 3 */
        } else {
            /* Only odd candidates after 2. */
            while (!is_prime(next_candidate))
                next_candidate += 2;
            p = next_candidate;
            next_candidate += 2;             /* advance to the next odd number */
        }

        /* Increment the prime index (k). */
        ++prime_index;                       /* prime_index now equals k */

        /* Test the required condition. */
        if (is_prime(p + (idx_t)50)) {
            ensure_terms(terms_cnt + 1);
            terms[terms_cnt++] = (seq_t)prime_index; /* store k as a term */
        }
    }
}

/* Return the n-th term of the sequence.  n >= 1. */
seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)0;           /* undefined but harmless */
    generate_up_to(n);
    return terms[(size_t)n - 1];
}
