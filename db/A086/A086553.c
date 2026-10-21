/*
 * A086553  a(1) = 1, a(n) = smallest number not included earlier such that
 *          both a(n) + 1 and a(n)*a(n-1) + 1 are primes.
 * https://oeis.org/A086553
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 43, u32 n <= 56, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes OEIS sequence A086553:
 * a(1) = 1; for n > 1,
 * a(n) = smallest positive integer not used in the sequence so far
 *        such that both a(n)+1 and a(n)*a(n-1)+1 are prime numbers.
 * The implementation uses a brute‑force search with memoisation.
 */
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/is_prime_big.h"

/* Static arrays for memoisation */
static seq_t *cache      = NULL;   /* array of sequence values */
static size_t cache_len  = 0;      /* number of stored values */
static size_t cache_cap  = 0;      /* allocated capacity for cache */

static unsigned char *used   = NULL;   /* bitmap of integers that already appear in the sequence */
static size_t used_cap   = 0;          /* allocated capacity for used */

/* Extend the bitmap of used integers up to 'need' */
static void ensure_used_cap(idx_t need)
{
    if ((size_t)need < used_cap)
        return;
    size_t newcap = used_cap ? used_cap << 1 : 128;
    while (newcap <= (size_t)need)
        newcap <<= 1;
    unsigned char *tmp = (unsigned char *)realloc(used, newcap);
    if (!tmp)
        exit(1);
    for (size_t i = used_cap; i < newcap; ++i)
        tmp[i] = 0;
    used  = tmp;
    used_cap = newcap;
}

/* Extend the cache array to hold at least 'need' elements */
static void ensure_cache_cap(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t newcap = cache_cap ? cache_cap << 1 : 16;
    while (newcap <= need)
        newcap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
    if (!tmp)
        exit(1);
    cache = tmp;
    cache_cap = newcap;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined below offset, harmless */

    /* Initialise data structures on first call */
    if (!cache) {
        cache = (seq_t *)malloc(16 * sizeof(seq_t));
        if (!cache)
            exit(1);
        used = (unsigned char *)malloc(128 * sizeof(unsigned char));
        if (!used)
            exit(1);
        for (size_t i = 0; i < 128; ++i)
            used[i] = 0;
        used[1] = 1;              /* a(1) = 1 is used */
        cache[0] = (seq_t)1;
        cache_len = 1;
        cache_cap = 16;
        used_cap   = 128;
    }

    /* Generate values up to requested index */
    while (cache_len < n) {
        seq_t prev = cache[cache_len - 1];   /* a(n-1) */
        idx_t cand = 1;                      /* candidate a(n) */

        for (;;) {
            ensure_used_cap(cand);
            if (!used[(size_t)cand]) {
                seq_t cand_seq = (seq_t)cand;
                seq_t p1 = cand_seq + 1;
                seq_t p2 = cand_seq * prev + 1;
                if (is_prime_big(p1) && is_prime_big(p2)) {
                    /* Found the next term of the sequence */
                    cache_len++;
                    ensure_cache_cap(cache_len);
                    cache[cache_len - 1] = cand_seq;
                    used[(size_t)cand] = 1;
                    break;
                }
            }
            ++cand;
        }
    }

    return cache[n - 1];
}
