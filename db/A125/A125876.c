/*
 * A125876  Prime numbers n such that cos(2pi/n) is an algebraic number of a
 *          13-smooth degree, but not 11-smooth.
 * https://oeis.org/A125876
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A125876 – primes p such that (p-1)/2 is 13‑smooth.
   The degree of the minimal polynomial of cos(2π/p) is (p-1)/2,
   so we just generate primes of the form p = 26·t + 1 where
   t is a 13‑smooth number.  The sequence is obtained by
   trial‑dividing each candidate and collecting every prime found.
   It starts 53,79,131,157,313,…
*/

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

/* 13‑smooth primes contributing to the humble numbers  */
static const uint8_t HUME_PRIMES[6] = {2, 3, 5, 7, 11, 13};

/* State for the humble‑number (13‑smooth) generator */
static size_t hume_ptr[6]   = {0, 0, 0, 0, 0, 0};   /* indices into humble[]   */
static unsigned long long *hume = NULL;              /* array of generated t's   */
static size_t hume_len   = 0;                        /* number of elements      */
static size_t hume_cap   = 0;                        /* allocated capacity      */

/* State for the list of qualifying primes */
static seq_t *primes = NULL;                        /* stored primes           */
static size_t pr_len   = 0;                          /* number of primes found  */
static size_t pr_cap   = 0;                          /* allocated capacity      */

static void grow_hume(size_t need)
{
    while (hume_cap < need) {
        size_t new_cap = (hume_cap == 0) ? 16 : hume_cap * 2;
        unsigned long long *tmp = (unsigned long long *)realloc(hume, new_cap * sizeof(unsigned long long));
        if (!tmp) abort();
        hume = tmp;
        hume_cap = new_cap;
    }
}

static void grow_primes(size_t need)
{
    while (pr_cap < need) {
        size_t new_cap = (pr_cap == 0) ? 16 : pr_cap * 2;
        seq_t *tmp = (seq_t *)realloc(primes, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        primes = tmp;
        pr_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* behaviour for n < 1 is irrelevant */

    /* initialise humble list if not yet done */
    if (hume_len == 0) {
        grow_hume(1);
        hume[0] = 1ULL;          /* the first 13‑smooth number */
        hume_len = 1;
    }

    /* generate primes until we have at least n of them */
    while (pr_len < (size_t)n) {
        /* produce next humble number (next smallest 13‑smooth) */
        unsigned long long next = 0ULL;
        for (size_t i = 0; i < 6; ++i) {
            unsigned long long cand = hume[hume_ptr[i]] * (unsigned long long)HUME_PRIMES[i];
            if (next == 0ULL || cand < next)
                next = cand;
        }

        /* append the new humble number */
        grow_hume(hume_len + 1);
        hume[hume_len++] = next;

        /* advance pointers that produced the chosen candidate */
        for (size_t i = 0; i < 6; ++i) {
            unsigned long long cand = hume[hume_ptr[i]] * (unsigned long long)HUME_PRIMES[i];
            if (cand == next)
                ++hume_ptr[i];
        }

        /* candidate prime p = 26 * t + 1 */
        unsigned long long t = next;
        seq_t p = (seq_t)26 * (seq_t)t + 1;

        /* test primality; only keep those that fit in idx_t */
        if (is_prime((idx_t)p)) {
            grow_primes(pr_len + 1);
            primes[pr_len++] = p;
        }
    }

    return primes[n - 1];
}
