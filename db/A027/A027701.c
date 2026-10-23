/*
 * A027701  Primes with even number of 1's in binary expansion such that next
 *          prime also has even number of 1's.
 * https://oeis.org/A027701
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 1001, u32 n <= 1001, u64 n <= 1001,
 *            u128 n <= 1001, big any n
 * Helpers:   is_prime_big, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: generate primes p with even popcount and the immediate next prime q
   also having even popcount.  Cache results for repeated calls.  Not a closed form. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime_small.h"
#include "../utils/is_prime_big.h"

/* popcount for a seq_t value, works for built‑in or arbitrary‑precision
   types that support &, >> and == 0 */
static seq_t popcount_seq(seq_t x)
{
    seq_t c = 0;
    while (x) {
        c = c + (x & 1);
        x = x >> 1;
    }
    return c;
}

/* prime test dispatching to the appropriate routine */
static bool is_prime_seq(seq_t x)
{
    if (sizeof(seq_t) <= sizeof(idx_t))
        return is_prime_small((idx_t)x);
    else
        return is_prime_big(x);
}

/* find the next prime strictly greater than 'start' (inclusive of start+1) */
static seq_t next_prime_after(seq_t start)
{
    seq_t p = start + 1;
    while (!is_prime_seq(p))
        p = p + 1;
    return p;
}

/* cache storage */
static seq_t *cache          = NULL;
static size_t cache_len      = 0;
static size_t cache_cap      = 0;
static seq_t first_candidate = 3;    /* start with the first odd prime */

/* grow cache to at least min_cap elements */
static void ensure_cache_cap(size_t min_cap)
{
    if (cache_cap >= min_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (new_cap < min_cap)
        new_cap = min_cap;
#if defined(__cplusplus)
    cache = reinterpret_cast<seq_t*>(realloc(cache, new_cap * sizeof(seq_t)));
#else
    cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
#endif
    if (!cache)
        exit(1);   /* allocation failed */
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* generate terms until we have at least n entries */
    while (cache_len < (size_t)n)
    {
        seq_t cand = first_candidate;

        if (is_prime_seq(cand))
        {
            seq_t pc_cand = popcount_seq(cand);
            if ((pc_cand & 1) == 0) /* even popcount for candidate */
            {
                seq_t nextp = next_prime_after(cand);
                seq_t pc_next = popcount_seq(nextp);
                if ((pc_next & 1) == 0)   /* even popcount for next prime */
                {
                    ensure_cache_cap(cache_len + 1);
                    cache[cache_len++] = cand;
                }
            }
        }

        first_candidate = first_candidate + 2; // skip even numbers
    }

    return cache[(size_t)n - 1];
}
