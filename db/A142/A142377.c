/*
 * A142377  Primes congruent to 26 mod 47.
 * https://oeis.org/A142377
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   nth prime ≡ 26 (mod 47); computed by search
 * Range:     u8 n <= 2, u16 n <= 143, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 57 terms through n = 949 (2026-11-03)
 * Helpers:   is_prime, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/mem.h"

/* Helper to grow a sequence array of type seq_t. */
static void grow_seq_array(seq_t **arr, size_t *size, size_t new_len)
{
    if (*size >= new_len) return;
    size_t new_cap = (*size > 0) ? (*size << 1) : 16;
    while (new_cap < new_len) new_cap <<= 1;
    seq_t *tmp = mem_resize(seq_t, *arr, new_cap);
    if (!tmp) abort();
    *arr = tmp;
    *size = new_cap;
}

/* Return the nth prime p such that p ≡ 26 (mod 47).
   Indexing starts at a(1)=73. The function caches results so
   that calls with arbitrary ordering stay fast. */
seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;   /* cached primes */
    static size_t cap     = 0;      /* current allocated capacity */
    static size_t len     = 0;      /* number of primes loaded */
    static idx_t  cand    = 73;     /* first candidate ≡ 26 (mod 47) */

    if (n < 1) return 0;            /* undefined for n < 1 */

    while (len < (size_t)n) {
        if (is_prime(cand)) {
            /* grow the array if necessary */
            grow_seq_array(&cache, &cap, len + 1);
            cache[len++] = (seq_t)cand;
        }
        cand += 47;                 /* next number ≡ 26 (mod 47) */
    }
    return cache[n - 1];
}
