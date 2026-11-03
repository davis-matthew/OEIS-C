/*
 * A104278  Numbers m such that 2m+1 and 2m-1 are not primes.
 * https://oeis.org/A104278
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 38, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 74 terms through n = 8993 (2026-11-03)
 * Helpers:   is_prime_seq, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A104278 – Numbers m such that 2m+1 and 2m-1 are not primes.
   The function generates terms on demand by testing successive integers
   with isprime_big and stores the results in a static cache.  The
   implementation uses the supplied utilities push_seq and isprime_big.  */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime_seq.h"
#include "../utils/mem.h"

/* Dynamic array helpers. */
static void push_seq(seq_t **arr, idx_t *cap, idx_t *size, seq_t val)
{
    if (*size >= *cap) {
        size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
        *arr = mem_resize(seq_t, *arr, new_cap);
        *cap = new_cap;
    }
    (*arr)[*size] = val;
    (*size)++;
}

seq_t a(idx_t n)
{
    /* static state: cached terms and the next integer to test */
    static seq_t *cache = NULL;
    static idx_t cnt   = 0;        /* number of terms cached */
    static idx_t cap   = 0;        /* current capacity of cache */
    static idx_t next  = 1;        /* next m to test */

    /* generate terms until we have at least n of them */
    while (cnt < n)
    {
        seq_t m   = (seq_t)next;                 /* current m */
        seq_t v1  = (seq_t)2 * m - 1;            /* 2m-1 */
        seq_t v2  = (seq_t)2 * m + 1;            /* 2m+1 */

        bool p1   = is_prime_seq(v1);             /* primality test */
        bool p2   = is_prime_seq(v2);

        if (!p1 && !p2)                         /* composite both sides */
            push_seq(&cache, &cap, &cnt, m);     /* add to cache */

        next++;                                  /* try next m */
    }

    return cache[n - 1];                          /* a(1) is first term */
}
