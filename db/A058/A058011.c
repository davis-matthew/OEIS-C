/*
 * A058011  Consider the sequence {b(m)} of nonprimes; sequence gives values
 *          of m where gcd{m, b(m)} increases.
 * https://oeis.org/A058011
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th non‑prime m such that gcd(m, b(m)) increases,
 *            where b(m) is the m‑th non‑prime number (non‑primes are listed
 *            in increasing order, starting with 1). The sequence is
 *            1,4,6,8,27,…
 * Range:     u8 n <= 9, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   gcd, isPrime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/isPrime.h"   /* bool isPrime(idx_t x);            */
#include "../utils/gcd.h"       /* seq_t gcd(seq_t a, seq_t b);      */

seq_t a(idx_t n) {
    /* Static storage to memoise results between calls. */
    static seq_t *cache      = NULL;  /* a(k) values   */
    static size_t cache_len   = 0;    /* how many stored   */
    static size_t cache_cap   = 0;    /* allocated size   */

    static idx_t  next_num   = 1;     /* next integer to test */
    static idx_t  m_count    = 0;     /* number of non‑primes seen (i.e. m) */
    static seq_t  prev_gcd   = 0;     /* last recorded gcd   */

    /* Ensure cache can hold the requested index. */
    if (cache_len <= n) {
        /* Grow the cache array if it hasn't been allocated yet. */
        if (!cache) {
            cache_cap = 64;
            cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        }
        /* Extend cache until we have a(n). */
        while (cache_len <= n) {
            /* Find the next non‑prime number. */
            while (true) {
                if (!isPrime(next_num)) {           /* non‑prime found   */
                    m_count++;                      /* index of current non‑prime */
                    seq_t val = (seq_t)next_num;
                    seq_t g   = gcd((seq_t)m_count, val);

                    if (g > prev_gcd) {             /* record new maximum */
                        if (cache_len == cache_cap) { /* grow cache */
                            cache_cap <<= 1;
                            cache = (seq_t *)realloc(cache,
                                                    cache_cap * sizeof(seq_t));
                        }
                        cache[cache_len++] = val;
                        prev_gcd = g;
                    }
                    /* Advance to the next integer for subsequent searches. */
                    ++next_num;
                    break;                          /* proceed to next iteration */
                }
                ++next_num;                         /* try the next integer   */
            }
        }
    }
    return cache[n];
}
