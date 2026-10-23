/*
 * A140023  Primes of the form 21x^2+65y^2.
 * https://oeis.org/A140023
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 207, u32 n <= 40, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A140023: Primes of the form 21*x^2 + 65*y^2
 *  The function searches primes congruent to one of 36 residues mod 5460
 *  which are the exact residues for which a prime can be represented
 *  by the quadratic form.  Candidates are tested for primality via
 *  is_prime_big.  A cache of found primes is maintained so that calls
 *  in any order are answered in O(1) after an overall linear scan. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"

/* residues modulo 5460 that represent primes of the form 21x^2+65y^2 */
static const uint16_t residues[36] = {
    149, 281, 401, 449, 821, 869, 1061, 1229,
    1241, 1289, 1409, 1541, 1649, 1709, 1961, 2069,
    2321, 2381, 2489, 2501, 2741, 2801, 2969, 3089,
    3161, 3581, 3749, 4061, 4181, 4349, 4649, 4769,
    4841, 5189, 5429, 5441
};

static seq_t *prime_cache = NULL;   /* array of discovered primes      */
static size_t cache_cnt  = 0;        /* number of primes stored         */
static size_t cache_cap  = 0;        /* allocated capacity              */
static idx_t  last_checked = 1;     /* last integer examined (0 -> 1) */

/* Ensure the cache has room for at least min_cap elements */
static void ensure_cache_capacity(size_t min_cap) {
    if (min_cap <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap : 64;
    while (new_cap < min_cap) new_cap <<= 1;
    seq_t *new_ptr = (seq_t *)realloc(prime_cache, new_cap * sizeof(seq_t));
    if (!new_ptr) abort();           /* allocation failed */
    prime_cache = new_ptr;
    cache_cap   = new_cap;
}

/* Extend the cache until it contains at least n primes */
static void grow_cache_to(idx_t n) {
    while (cache_cnt < n) {
        /* advance to next integer candidate */
        ++last_checked;

        /* skip even numbers */
        if ((last_checked & 1ULL) == 0) continue;

        /* modulo 5460 residue test */
        uint16_t rem = (uint16_t)(last_checked % 5460ULL);
        bool ok = false;
        for (size_t i = 0; i < 36; ++i) {
            if (rem == residues[i]) { ok = true; break; }
        }
        if (!ok) continue;            /* not a representable residue */

        /* primality test */
        seq_t cand = (seq_t)last_checked;
        if (!is_prime_big(cand)) continue;

        /* store prime */
        ensure_cache_capacity(cache_cnt + 1);
        prime_cache[cache_cnt++] = cand;
    }
}

/* External API required by the test harness */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;          /* undefined for n=0, return 0 */
    if (cache_cnt < n) grow_cache_to(n);
    return prime_cache[n - 1];
}
