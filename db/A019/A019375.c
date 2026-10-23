/*
 * A019375  Primes with primitive root 51.
 * https://oeis.org/A019375
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   factor_u64, is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A019375: primes p for which 51 is a primitive root modulo p.
   The implementation tests each prime by checking that the order of 51
   modulo that prime equals p-1.  Results are cached for fast
   subsequent calls. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "../utils/is_prime.h"
#include "../utils/factor_u64.h"
#include "../utils/mod_pow.h"

/* --- static cache for generated terms ---------------------------------- */
static seq_t *cache = NULL;          /* stored terms */
static size_t cache_len = 0;         /* number of stored terms */
static size_t cache_cap = 0;         /* allocated capacity */

/* next integer to test (starts at 2) */
static uint64_t next_candidate = 2;

/* Ensure the cache has enough space for one more element. */
static void ensure_cache_capacity(void)
{
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* In an OEIS build this never happens for the tested range. */
            exit(1);
        }
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Append a new term to the cache. */
static void append_cache(seq_t val)
{
    ensure_cache_capacity();
    cache[cache_len++] = val;
}

/* Return the n‑th term of the sequence (1‑based index). */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;              /* out of range, not used by OEIS */

    while (cache_len < (size_t)n) {
        /* --- Find next prime candidate -------------------------------- */
        while (!is_prime((idx_t)next_candidate))
            next_candidate += (next_candidate == 2) ? 1 : 2; /* skip evens */

        bool good = true;

        /* 51 must be coprime to p (skip 3 and 17). */
        if (next_candidate == 3 || next_candidate == 17)
            good = false;
        else {
            /* Factor p-1 */
            uint64_t pf[64], ef[64];
            size_t pf_cnt = 0;
            factor_u64(next_candidate - 1, pf, ef, &pf_cnt);

            for (size_t i = 0; i < pf_cnt && good; ++i) {
                uint64_t q = pf[i];
                idx_t exp = (idx_t)((next_candidate - 1) / q);
                seq_t r = mod_pow((seq_t)51, exp, (seq_t)next_candidate);
                if (r == (seq_t)1)   /* not a primitive root */
                    good = false;
            }
        }

        if (good)
            append_cache((seq_t)next_candidate);

        /* Advance to next odd integer (after 2) */
        if (next_candidate == 2)
            next_candidate = 3;
        else
            next_candidate += 2;
    }

    return cache[n - 1];
}
