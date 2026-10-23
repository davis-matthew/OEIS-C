/*
 * A132251  Isolated primes congruent to 17 (mod 30).
 * https://oeis.org/A132251
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function generates OEIS sequence A132251.
   It searches for primes congruent to 17 (mod 30) that are not part of a
   twin‑prime pair (i.e., both p–2 and p+2 are composite).  The algorithm
   works by iterating over the arithmetic progression 30·k+17 for k ≥ 1,
   testing each candidate for primality and for the absence of twin primes.
   The results are memoised in a static array so that calls in any order
   are O(1) after the first discovery. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"   /* provides bool is_prime_big(seq_t) */

static seq_t *cache = NULL;          /* memoised terms */
static size_t cache_cap = 0;
static idx_t  cache_n   = 0;         /* number of terms stored */
static idx_t  cand_k    = 1;         /* next k value for candidate 30·k+17 */

/* Ensure the cache can hold at least `need` elements. */
static void ensure_cap(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = (cache_cap == 0 ? 64 : cache_cap << 1);
    while (new_cap < need) new_cap <<= 1;
    /* cast the void* returned by realloc to seq_t* for C++ compatibility */
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* Return the n‑th isolated prime ≡ 17 (mod 30) (1‑based index). */
seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* undefined for n == 0, defensive guard */

    while (cache_n < n) {
        seq_t cand = ((seq_t)30 * cand_k) + 17;

        if (is_prime_big(cand)) {
            /* candidate is prime; check that it is not in a twin pair */
            seq_t cand_minus_2 = cand - 2;
            seq_t cand_plus_2  = cand + 2;
            if (!is_prime_big(cand_minus_2) && !is_prime_big(cand_plus_2)) {
                ensure_cap(cache_n + 1);
                cache[cache_n++] = cand;
            }
        }
        cand_k++;   /* move to the next potential candidate */
    }
    return cache[n - 1];
}
