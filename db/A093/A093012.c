/*
 * A093012  Numbers k such that prime(k) == 3 (mod 16).
 * https://oeis.org/A093012
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A093012: the indices k for which the k‑th prime is ≡ 3 (mod 16).
   The routine incrementally generates primes, collects the qualifying
   indices, and returns the n‑th one.  For arbitrary‑precision seq_t
   the indices are stored as uint64_t and cast to seq_t on return. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* storage for primes found so far */
static uint64_t *prime_list = NULL;
static size_t   prime_cnt  = 0;
static size_t   prime_cap  = 0;

/* storage for qualifying indices (as uint64_t) */
static uint64_t *qual = NULL;
static size_t    qual_cnt = 0;
static size_t    qual_cap = 0;

static uint64_t cur_num = 2;           /* next natural number to test */

/* Ensure prime_list can hold at least min_cap elements */
static void grow_prime_array(size_t min_cap)
{
    if (prime_cap < min_cap) {
        size_t new_cap = prime_cap ? prime_cap << 1 : 8;
        if (new_cap < min_cap) new_cap = min_cap;
        uint64_t *tmp = (uint64_t *)realloc(prime_list, new_cap * sizeof(uint64_t));
        prime_list = tmp;
        prime_cap  = new_cap;
    }
}

/* Ensure qual can hold at least min_cap elements */
static void grow_qual_array(size_t min_cap)
{
    if (qual_cap < min_cap) {
        size_t new_cap = qual_cap ? qual_cap << 1 : 8;
        if (new_cap < min_cap) new_cap = min_cap;
        uint64_t *tmp = (uint64_t *)realloc(qual, new_cap * sizeof(uint64_t));
        qual = tmp;
        qual_cap = new_cap;
    }
}

/* Trial division primality test, using already found primes */
static bool is_prime_candidate(uint64_t n)
{
    if (n < 2) return false;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        if (p > n / p) break;          /* p*p > n */
        if (n % p == 0) return false;
    }
    return true;
}

/* Generate primes and qualifying indices until we have at least n qualifiers */
static void ensure_qualifiers_for_n(idx_t n)
{
    while (qual_cnt < (size_t)n) {
        uint64_t cand = cur_num;
        cur_num++;                           /* advance candidate for next iteration */

        if (is_prime_candidate(cand)) {
            /* store the prime itself */
            grow_prime_array(prime_cnt + 1);
            prime_list[prime_cnt++] = cand;

            /* index of this prime is prime_cnt after increment */
            if ((cand & 15ULL) == 3ULL) {   /* cand % 16 == 3 */
                uint64_t idx = prime_cnt;  /* 1‑based index of this prime */
                grow_qual_array(qual_cnt + 1);
                qual[qual_cnt++] = idx;
            }
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;     /* undefined; safety only */
    ensure_qualifiers_for_n(n);
    return (seq_t)qual[n - 1];
}
