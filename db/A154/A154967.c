/*
 * A154967  Nonprime numbers n such that concatenation of n and n-th prime is
 *          composite.
 * https://oeis.org/A154967
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, less
 * Formula:   a(n) is the n-th nonprime number k such that the concatenation
 *            of k with the k-th prime is composite
 * Range:     u8 n <= 3, u16 n <= 26, u32 n <= 44, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   concat_parts, get_prime, is_prime, is_prime_big, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compile with the helper utilities as provided by the build system. */
#include <stddef.h>
#include <stdbool.h>

#include "../utils/is_prime.h"          /* bool is_prime(idx_t x); */
#include "../utils/get_prime.h"         /* uint32_t get_prime(idx_t k); */
#include "../utils/is_prime_big.h"/* bool is_prime_big(seq_t n); */
#include "../utils/concat_parts.h"     /* seq_t concat_parts(idx_t a, idx_t b); */
#include "../utils/seq_realloc.h"     /* void * seq_realloc(void *old, size_t old_cap, size_t new_cap); */

/* ======== caching of generated terms ===================== */
static seq_t *cache          = NULL;   /* array of already found terms   */
static size_t cache_len      = 0;      /* number of terms stored        */
static size_t cache_cap      = 0;      /* current allocation size       */
static idx_t  tested_up_to   = 0;      /* largest number that has been checked */

/* Extend the cache until it contains at least 'required' terms. */
static void
ensure_terms(idx_t required)
{
    while (cache_len < (size_t)required)
    {
        for (idx_t cand = tested_up_to + 1; ; ++cand)
        {
            /* is cand a nonprime?  (1 is considered nonprime) */
            bool nonprime = (cand == 1) || !is_prime(cand);
            if (!nonprime) {
                tested_up_to = cand;
                continue;
            }

            /* kth prime: get_prime(cand) into seq_t-friendly form */
            uint32_t pk = get_prime(cand);

            /* concatenate cand and its prime */
            seq_t concat = concat_parts(cand, (idx_t)pk);

            /* if the concatenation is not prime, cand belongs to the sequence */
            if (!is_prime_big(concat))
            {
                /* grow cache if necessary */
                if (cache_len == cache_cap)
                {
                    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
                    cache = (seq_t *)seq_realloc(cache, cache_cap, new_cap);
                    cache_cap = new_cap;
                }
                cache[cache_len++] = (seq_t)cand;
                tested_up_to = cand;
                break;  /* found the next term */
            }

            tested_up_to = cand;
        }
    }
}

/* ===================== public function =================== */
seq_t a(idx_t n)
{
    if (n == 0) {
        /* undefined for 0, return 0; sequence is offset 1 */
        return (seq_t)0;
    }
    /* generate enough terms */
    ensure_terms(n);
    /* seq_t is 0‑based in cache, but a(1) is the first stored element */
    return cache[n - 1];
}
