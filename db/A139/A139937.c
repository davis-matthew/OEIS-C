/*
 * A139937  Primes of the form 5x^2+69y^2.
 * https://oeis.org/A139937
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 414, u32 n <= 287, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt, grow_seq, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes primes of the form 5*x^2 + 69*y^2 by using the known
   residue classes modulo 1380 and testing each prime for
   representability with a brute‑force search of the quadratic
   form.  The function caches results to support arbitrary order
   of queries. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "../utils/is_prime_big.h"
#include "../utils/floor_sqrt.h"
#include "../utils/grow_seq.h"

/* Residue classes of primes of that form (mod 1380). */
static const int RESIDUES[] = {
    5, 89, 149, 221, 281, 329, 341, 389, 401,
    521, 569, 641, 689, 701, 881, 941, 1049,
    1109, 1121, 1169, 1229, 1241, 1349
};
#define RES_COUNT (sizeof RESIDUES / sizeof RESIDUES[0])

/* Cached list of found primes. */
static seq_t *prime_cache = NULL;
static idx_t  cache_len  = 0;
static size_t cache_cap  = 0;

/* State for generating candidates in order of increasing value. */
static idx_t next_k = 0;      /* multiplier of the period 1380 */
static size_t next_r = 0;     /* index in the residue array */

/* Test whether p = 5*x^2 + 69*y^2 for some integers x, y. */
static bool has_representation(seq_t p)
{
    for (idx_t y = 0;; ++y) {
        seq_t y2   = (seq_t)y * (seq_t)y;
        seq_t part = (seq_t)69 * y2;
        if (part > p) break;

        seq_t rem = p - part;
        if (rem % (seq_t)5 != 0) continue;

        seq_t x2 = rem / (seq_t)5;
        seq_t x  = floor_sqrt(x2);
        if (x * x == x2) return true;
    }
    return false;
}

/* Generate primes until we have at least n terms. */
static void ensure_terms(idx_t n)
{
    while (cache_len < n) {
        /* Next candidate number in the residue classes. */
        seq_t candidate =
            ((seq_t)1380 * (seq_t)next_k) + (seq_t)RESIDUES[next_r];

        /* Advance to the following residue. */
        ++next_r;
        if (next_r == RES_COUNT) {
            next_r = 0;
            ++next_k;
        }

        if (!is_prime_big(candidate))
            continue;

        if (!has_representation(candidate))
            continue;

        /* Store the new prime. */
        grow_seq(&prime_cache, &cache_cap, cache_len + 1);
        prime_cache[cache_len++] = candidate;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* undefined, but safe */
    ensure_terms(n);
    return prime_cache[n - 1];
}
