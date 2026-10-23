/*
 * A141178  Primes of the form 3*x^2+x*y-3*y^2 (as well as of the form
 *          3*x^2+7*x*y+y^2).
 * https://oeis.org/A141178
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th prime p for which the Jacobi symbol (37/p) = 1.
 *            (The prime p = 3 is included explicitly.) The function generates
 *            primes in increasing order and stores them in a static cache so
 *            calls in any order are supported
 * Range:     u8 n <= 26, u16 n <= 3262, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime, jacobi_symbol
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>           /* malloc / realloc */
#include "../utils/jacobi_symbol.h"
#include "../utils/is_prime.h"

/* Ensure |arr| >= needed.  Double the capacity until it is large enough. */
static void grow_seq_array(seq_t **arr, size_t *cap, size_t needed)
{
    if (*cap < needed) {
        size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
        while (new_cap < needed) new_cap <<= 1;
        *arr = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
        *cap = new_cap;
    }
}

/* Cached sequence of prime values satisfying the condition. */
static seq_t *seq_list   = NULL;   /* array of found primes */
static size_t seq_len    = 0;      /* number of elements stored */
static size_t seq_cap    = 0;      /* allocated capacity of seq_list */

/* The next integer that has not yet been tested for membership. */
static uint64_t current_candidate = 3;   /* start at the first prime in the sequence */

/* Extend the cache so that it contains at least n terms (1‑based). */
static void ensure_cache(idx_t n)
{
    if (seq_len >= (size_t)n) return;       /* already enough */

    while ((size_t)seq_len < n) {
        /* test current_candidate for primality */
        if (is_prime((idx_t)current_candidate)) {
            /* representability test: Jacobi(37,p)=1, prime 3 is allowed */
            if (current_candidate == 3 || current_candidate == 37 ||
                jacobi_symbol(37, current_candidate) == 1) {
                grow_seq_array(&seq_list, &seq_cap, seq_len + 1);
                seq_list[seq_len++] = (seq_t)current_candidate;
            }
        }
        /* advance to the next integer */
        ++current_candidate;
    }
}

/* Public entry point.  Returns the n‑th term of the sequence, 1‑based. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* behaviour for n < 1 is unspecified */
    ensure_cache(n);
    return seq_list[n - 1];
}
