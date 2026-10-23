/*
 * A127178  Primes whose "Look And Say" descriptions from right to left (in
 *          the sense of method B, i.e., digit-indication followed by
 *          frequency) are also primes.
 * https://oeis.org/A127178
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 7, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   descr_from_prime, is_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = the nth prime p such that the right‑to‑left
   "Look‑and‑Say" description of p (digit followed by run length)
   is also prime.  The function generates terms by searching primes
   and memoising them. */

#include <stdlib.h>
#include <stdbool.h>
/* Helper functions available from utils */
#include "../utils/is_prime.h"
#include "../utils/is_prime_big.h"
#include "../utils/descr_from_prime.h"

static seq_t *seq_cache = NULL;   /* memoised sequence values   */
static size_t seq_cap  = 0;       /* allocated capacity         */
static size_t seq_len  = 0;       /* number of terms computed   */
static idx_t  candidate = 2;      /* next integer to test      */

/* Return the nth term (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for n<1 */

    /* Generate terms until we have at least n terms */
    while (seq_len < n) {
        /* Find next prime */
        idx_t p = candidate;
        while (!is_prime(p)) ++p;

        /* Compute description and test primality of the description */
        seq_t descr = descr_from_prime(p);
        if (is_prime_big(descr)) {
            /* Add to memoised cache */
            if (seq_len == seq_cap) {
                seq_cap = seq_cap ? seq_cap * 2 : 16;
                seq_cache = (seq_t*)realloc(seq_cache, seq_cap * sizeof(seq_t));
                if (!seq_cache) return 0; /* allocation failure */
            }
            seq_cache[seq_len++] = (seq_t)p;
        }

        candidate = p + 1;
    }

    return seq_cache[n - 1];
}
