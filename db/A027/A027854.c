/*
 * A027854  Mutinous numbers: n > 1 such that n/p^k > p, where p is the
 *          largest prime dividing n and p^k is the highest power of p
 *          dividing n.
 * https://oeis.org/A027854
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gpf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Mutual numbers – sequence A027854
   a(n) is the nth integer n≥2 such that if p is the largest prime that
   divides n and n = p^k · m with p∤m, then m > p.
   The sequence starts 12,24,30,36,40,45,48,56,60,63,70,72,80,84,90,96,105,108,112,
   120,126,132,135,140,… */

/* This implementation keeps a memoised list of the sequence values.  For
   each candidate number it uses the helper function
   gpf(x) ("largest prime factor") – available in the utils library – to
   compute the largest prime divisor, the corresponding power, and checks
   whether the remaining co‑factor is greater than that prime.  The list
   is grown on demand with realloc, casting the void* result explicitly
   so that the code compiles under C++ as well.  */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/gpf.h"

static uint64_t *primes = NULL;        /* primes used for factorisation */
static size_t primes_cap  = 0;
static size_t primes_size = 0;
static uint64_t prime_max  = 1;        /* largest value tried for primality */

/* cache of the sequence values: first element is a(1) */
static seq_t *mutine_cache = NULL;
static size_t mutine_cap  = 0;
static size_t mutine_size = 0;
static uint64_t next_candidate = 2;   /* next integer to test */

/* ensure we have primes up to 'limit' – simple trial division */
static void ensure_primes_up_to(uint64_t limit)
{
    if (limit <= prime_max) return;
    for (uint64_t cand = prime_max + 1; cand <= limit; ++cand) {
        bool is_p = true;
        for (size_t i = 0; i < primes_size; ++i) {
            uint64_t p = primes[i];
            if ((unsigned __int128)p * p > cand) break;
            if (cand % p == 0) { is_p = false; break; }
        }
        if (is_p) {
            if (primes_size == primes_cap) {
                primes_cap = primes_cap ? primes_cap * 2 : 64;
                uint64_t *tmp = (uint64_t*)realloc(primes,
                                                   primes_cap * sizeof(uint64_t));
                if (!tmp) return; /* allocation failed – unlikely */
                primes = tmp;
            }
            primes[primes_size++] = cand;
        }
    }
    prime_max = limit;
}

/* append a new term to the cache, growing the array if necessary */
static void cache_append(uint64_t value)
{
    if (mutine_size == mutine_cap) {
        size_t new_cap = mutine_cap ? mutine_cap * 2 : 64;
        seq_t *tmp = (seq_t*)realloc(mutine_cache,
                                     new_cap * sizeof(seq_t));
        if (!tmp) return; /* allocation failed – unlikely */
        /* initialize the newly allocated part to zeros (good for py big ints) */
        for (size_t i = mutine_size; i < new_cap; ++i) tmp[i] = (seq_t)0;
        mutine_cache = tmp;
        mutine_cap   = new_cap;
    }
    mutine_cache[mutine_size++] = (seq_t)value;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;        /* undefined – return 0 for safety */
    while (mutine_size < (size_t)n) {
        /* find the next mutinous number */
        while (true) {
            uint64_t x = next_candidate;
            if (x <= 1) { next_candidate = x + 1; continue; }

            /* find largest prime factor */
            uint64_t p = gpf(x);
            if (!p) { next_candidate = x + 1; continue; }

            /* largest power of p dividing x */
            uint64_t temp = x;
            uint64_t power = 1;
            while (temp % p == 0) {
                power *= p;
                temp /= p;
            }

            /* m = x / p^k  ; check condition m > p */
            seq_t m = (seq_t)x / power;
            if (m > (seq_t)p) {
                cache_append(x);
                next_candidate = x + 1;
                break;          /* one more term added; back to outer loop */
            }
            next_candidate = x + 1;
        }
    }
    return mutine_cache[n - 1];
}
