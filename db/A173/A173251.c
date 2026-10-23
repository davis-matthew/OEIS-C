/*
 * A173251  Positive odd nonprimes of the form prime-6.
 * https://oeis.org/A173251
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A173251: a(n) = p − 6, where p is a prime ≥ 7,
   and p − 6 is an odd nonprime.  The function caches values
   so that calls do not need to be monotonic. */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

static seq_t *seq_cache = NULL;   /* cached sequence values */
static size_t seq_len   = 0;      /* number of values currently cached */
static size_t seq_cap   = 0;      /* capacity of seq_cache */
static uint64_t last_prime = 2;   /* last prime examined */

/* Ensure that seq_cache has space for at least 'needed' elements. */
static void ensure_seq_capacity(size_t needed)
{
    if (needed <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    if (new_cap < needed) new_cap = needed;
    seq_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    seq_cap   = new_cap;
}

/* Find the first prime ≥ start. */
static uint64_t next_prime_uint64(uint64_t start)
{
    if (start <= 2) return 2;
    if ((start & 1) == 0) ++start;            /* make odd */
    for (;; start += 2) {
        if (is_prime((idx_t)start)) return start;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* undefined for n < 1 */

    /* Grow cache to at least n elements */
    if (seq_len < (size_t)n) {
        ensure_seq_capacity((size_t)n);
        while (seq_len < (size_t)n) {
            uint64_t p = next_prime_uint64(last_prime + 1);
            last_prime = p;

            if (p < 7) continue;             /* p must be ≥ 7 */

            idx_t m_val = (idx_t)(p - 6);     /* p - 6 is odd */
            if (!is_prime(m_val)) {          /* nonprime odd? */
                seq_cache[seq_len++] = (seq_t)m_val;
            }
        }
    }

    return seq_cache[(size_t)(n - 1)];
}
