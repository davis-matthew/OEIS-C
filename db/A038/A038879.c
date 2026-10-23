/*
 * A038879  10 is a square mod p.
 * https://oeis.org/A038879
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime, jacobi_symbol
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes the OEIS sequence A038879: the list of primes
 * p such that 10 is a quadratic residue modulo p (including the
 * exceptional primes 2 and 5).  The implementation searches for
 * successive primes, testing each for primality with is_prime and
 * checking the Legendre symbol (10|p) using jacobi_symbol.  The
 * results are cached so that calls with arbitrary ordering of n are
 * efficient. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/jacobi_symbol.h"
#include "../utils/is_prime.h"

static seq_t *prime_cache = NULL;
static size_t cache_len   = 0;
static size_t cache_cap   = 0;
static idx_t  last_checked = 0;

seq_t a(idx_t n) {
    if (n == 0) return 0;            /* undefined but safe guard */
    while (cache_len < n) {
        /* advance to the next candidate */
        if (last_checked < 2) {
            last_checked = 2;
        } else if (last_checked == 2) {
            last_checked = 3;
        } else if (last_checked == 3) {
            last_checked = 5;
        } else if (last_checked == 5) {
            last_checked = 7;
        } else {
            last_checked += 2;     /* skip even numbers */
        }

        /* primality test */
        if (!is_prime(last_checked))
            continue;

        /* test the condition for inclusion in the sequence */
        if (last_checked == 2 ||
            last_checked == 5 ||
            jacobi_symbol(10ULL, (uint64_t)last_checked) == 1) {

            /* ensure capacity */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? (cache_cap << 1) : 8;
                seq_t *tmp = (seq_t *)realloc(prime_cache,
                                              new_cap * sizeof(seq_t));
                if (!tmp) return 0;   /* allocation failure */
                prime_cache = tmp;
                cache_cap   = new_cap;
            }
            prime_cache[cache_len++] = (seq_t)last_checked;
        }
    }
    return prime_cache[n - 1];
}
