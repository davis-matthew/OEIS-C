/*
 * A202110  Numbers k such that 90*k + 7 is prime.
 * https://oeis.org/A202110
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th integer k such that 90*k + 7 is prime
 * Range:     u8 n <= 2, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Validated: partial, pass, 63 terms through n = 63 (2026-11-03)
 * Helpers:   is_prime_seq, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_seq.h"
#include "../utils/mem.h"

/* Return the n-th term (1‑based) of sequence A202110. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* out of range – undefined */
    static seq_t *cache  = NULL;                /* stored k values */
    static idx_t  len    = 0;                   /* number of cached terms */
    static idx_t  cap    = 0;                   /* allocated capacity */
    static idx_t  next_k = 0;                   /* next candidate k to test */

    /* Allocate or grow the cache as needed. */
    if (!cache) {
        cap   = 128;
        cache = mem_new(seq_t, cap);
    }

    /* Generate terms until we have at least n of them. */
    while (len < n) {
        seq_t candidate = (seq_t)90 * (seq_t)next_k + (seq_t)7;
        /* For k values that stay within 64 bits, is_prime_small is fast. */
        if (is_prime_seq((idx_t)candidate)) {
            if (len >= cap) {
                cap *= 2;
                cache = mem_resize(seq_t, cache, cap);
            }
            cache[len++] = (seq_t)next_k;
        }
        ++next_k;
    }

    return cache[n - 1];
}
