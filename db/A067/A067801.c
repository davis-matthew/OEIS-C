/*
 * A067801  Numbers m such that bigomega(m)=2*omega(m).
 * https://oeis.org/A067801
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   numbers m such that bigomega(m) = 2 * omega(m)
 * Range:     u8 n <= 26, u16 n <= 3576, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   bigomega, count_distinct_primes, ensure_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A067801 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/bigomega.h"
#include "../utils/count_distinct_primes.h"
#include "../utils/ensure_capacity.h"

/* The sequence is generated lazily: we store the terms in a static array
   and extend it as needed. Subsequent calls reuse the cache, so the
   function can be called out of order without recomputation. */
static seq_t *seq_cache = NULL;
static size_t seq_len  = 0;   /* number of terms cached */
static size_t seq_cap  = 0;   /* capacity of seq_cache */

/* Append a new element to the cache, resizing as necessary. */
static void append_term(seq_t val)
{
    if (seq_len >= seq_cap)
        ensure_capacity(seq_len + 1, &seq_cap, (void**)&seq_cache,
                        sizeof(seq_t));
    seq_cache[seq_len++] = val;
}

/* Compute the nth term of the sequence. Indexing starts at 1. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;    /* undefined, but keep behaviour sane */

    /* Expand the cache until it contains at least n terms. */
    if (seq_len < n) {
        /* Start scanning from the number after the last cached term.
           For the first call we start at 1. */
        uint64_t cur = (seq_len == 0) ? 1ULL : (uint64_t)seq_cache[seq_len - 1] + 1;
        /* Scan forward until we have enough terms. */
        while (seq_len < n) {
            seq_t bo = bigomega(cur);                      /* total prime multiplicity */
            int   ol = count_distinct_primes(cur);         /* distinct primes */
            if (bo == (seq_t)(ol * 2U))
                append_term((seq_t)cur);
            cur++;
        }
    }
    return seq_cache[n - 1];
}
