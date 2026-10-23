/*
 * A141832  Integers n>1 such that A141822(n)=2.
 * https://oeis.org/A141832
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 112, u16 n <= 4398, u32 n <= 4398, u64 n <= 4398,
 *            u128 n <= 4398, big any n
 * Helpers:   cf_bounded_two
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A141832: integers n>1 such that A141822(n)=2.
 * Here A141822(n) is the minimal integer B such that there exists a
 * coprime numerator p with all quotients of the Euclidean algorithm
 * for p/n ≤ B.  For A141822(n)=2 we test whether there is a coprime
 * p with all continued‐fraction terms ≤2.  The function
 * seq_t a(idx_t n) returns the n-th term (1‑based offset).
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/cf_bounded_two.h"

/* Euclidean gcd for uint64_t, used only by the test function. */
static inline idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b) {
        idx_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

/* Return true if there exists a coprime p<n with cf_bounded_two(p,n). */
static bool minimal_max_equals_two(idx_t n)
{
    for (idx_t p = 1; p < n; ++p) {
        if (gcd_idx(p, n) != 1)
            continue;
        if (cf_bounded_two(p, n))
            return true;
    }
    return false;
}

/* Cache for previously computed terms.  Stored as a dynamic array. */
static seq_t *seq_cache = NULL;
static idx_t seq_len  = 0;   /* number of terms cached, 1‑based offset. */
static size_t seq_cap = 0;   /* current capacity of seq_cache. */

/* Return the n-th term of the sequence.  n≥1. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* safety guard for offset 1 */

    /* Grow cache until we have at least n terms. */
    while (seq_len < n) {
        /* Find the next integer satisfying the property. */
        idx_t candidate;
        if (seq_len == 0)
            candidate = 2;           /* first term is 2 */
        else
            candidate = (idx_t)seq_cache[seq_len - 1] + 1;

        while (!minimal_max_equals_two(candidate))
            ++candidate;

        /* Allocate more space if necessary. */
        if (seq_len == seq_cap) {
            size_t new_cap = (seq_cap == 0) ? 8 : seq_cap * 2;
            /* realloc returns void* – cast explicitly for C/C++ compatibility. */
            seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
            seq_cache = tmp;
            seq_cap   = new_cap;
        }

        /* Store the found value. */
        seq_cache[seq_len++] = (seq_t)candidate;
    }

    return seq_cache[n - 1];
}
