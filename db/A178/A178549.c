/*
 * A178549  a(n) is a composite number at the start of an interval of
 *          consecutive integers, ending in a prime, and non-overlapping with
 *          and at least as long as the interval addressed by a(n-1).
 * https://oeis.org/A178549
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: each a(n) is the start of the n-th non‑overlapping
 * interval of consecutive integers that begins with a composite number,
 * ends with a prime number, and whose length is at least as large as the
 * length of the previous interval.  The intervals are constructed
 * greedily and stored for fast repeated evaluation. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          // offset is 1; values for n<1 are undefined

    /* Dynamically grow these arrays as needed. */
    static seq_t *starts   = NULL; /* a(k) values (start of each interval) */
    static idx_t *lengths = NULL; /* corresponding interval lengths */
    static idx_t stored   = 0;     /* number of entries already computed */
    static idx_t capacity = 0;     /* current allocation size */

    /* Ensure enough space for at least n terms. */
    if (n > stored) {
        if (n > capacity) {
            idx_t newcap = capacity ? capacity * 2 : 1;
            if (newcap < n) newcap = n;
            seq_t *tmp1 = (seq_t*)realloc(starts, newcap * sizeof(seq_t));
            if (!tmp1) abort();
            starts   = tmp1;
            idx_t *tmp2 = (idx_t*)realloc(lengths, newcap * sizeof(idx_t));
            if (!tmp2) abort();
            lengths = tmp2;
            capacity = newcap;
        }

        /* Compute missing terms up to n. */
        for (idx_t i = stored; i < n; ++i) {
            if (i == 0) {               /* a(1) = 4, length 2  */
                starts[0]   = 4;
                lengths[0]  = 2;
                continue;
            }

            seq_t prev_start = starts[i-1];
            idx_t  prev_len  = lengths[i-1];
            seq_t  start     = prev_start + (seq_t)prev_len; /* next interval start */

            /* Find the first prime at or beyond start + prev_len - 1. */
            seq_t candidate = start + (seq_t)prev_len - 1;
            while (!is_prime_big(candidate)) ++candidate;

            seq_t  end = candidate;
            seq_t  len = end - start + 1;          /* interval length */

            starts[i]  = start;
            lengths[i] = (idx_t)len;                /* len fits in idx_t for all n considered */
        }
        stored = n;
    }

    return starts[n-1];
}
