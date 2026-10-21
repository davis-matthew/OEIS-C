/*
 * A054706  Number of powers of 5 modulo n.
 * https://oeis.org/A054706
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A054706 - Number of powers of 5 modulo n
 *
 *  a(n) = #{ 5^k mod n : k ≥ 0 }   (distinct residues)
 *  The algorithm builds the orbit {1,5,5²,…} modulo n and counts
 *  distinct values until a repetition occurs.  The result is cached
 *  so each value is computed at most once.
 *
 *  Complexity per call is O(λ(n)) where λ is the Carmichael
 *  function.  For n ≤ 10 000 this is well below the 2‑second limit.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/* cached results – 0 means not yet computed (a(n) ≥ 1 for n≥1) */
static seq_t *a_cache = NULL;
static size_t  a_cache_size = 0;

/* temporary visited array for residues modulo n.  Re‑used for all calls. */
static unsigned char *visited = NULL;
static size_t  visited_size = 0;

seq_t a(idx_t n)
{
    /* protect against invalid input */
    if (n == 0)  return 0;  /* undefined; return 0 */

    /* enlarge cache if necessary */
    if ((size_t)n >= a_cache_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)realloc(a_cache, new_size * sizeof(seq_t));
        if (!new_cache)  return 0;                  /* out of memory */
        /* initialise new entries to 0 */
        memset(new_cache + a_cache_size, 0,
               (new_size - a_cache_size) * sizeof(seq_t));
        a_cache = new_cache;
        a_cache_size = new_size;
    }

    /* return cached value if already known */
    if (a_cache[n] != 0)  return a_cache[n];

    /* enlarge visited array if necessary */
    if ((size_t)n > visited_size) {
        size_t new_vis = (size_t)n;
        unsigned char *new_visited = (unsigned char *)realloc(visited,
                                 new_vis * sizeof(unsigned char));
        if (!new_visited)  return 0;                 /* out of memory */
        visited = new_visited;
        visited_size = new_vis;
    }

    /* clear visited markers for this n */
    memset(visited, 0, (size_t)n * sizeof(unsigned char));

    /* iterate powers of 5 modulo n */
    size_t count = 0;
    unsigned long long cur = 1 % n;  /* 5^0 mod n */
    while (!visited[(size_t)cur]) {
        visited[(size_t)cur] = 1;
        ++count;
        cur = (cur * 5ULL) % n;          /* 5^(k+1) mod n */
    }

    a_cache[n] = (seq_t)count;   /* cache and return */
    return a_cache[n];
}
