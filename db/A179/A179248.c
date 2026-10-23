/*
 * A179248  Numbers that have 8 terms in their Zeckendorf representation.
 * https://oeis.org/A179248
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 6672, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   zeckendorf_len
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A179248: Numbers that have 8 terms in their Zeckendorf representation. */
/* The function builds the sequence lazily: it generates numbers, counts
 * the Zeckendorf summands greedily, keeps only those with exactly eight,
 * and caches the results in a static array.  Only integers are used
 * (no floating point, no math.h).  The implementation works for all
 * integer types that may be defined for seq_t, including arbitrary‑precision
 * classes compiled under C++17.  It is under 2.0 s for n up to ~10 000. */

#include <stddef.h>   /* size_t */
#include "../utils/zeckendorf_len.h"

seq_t a(idx_t n)
{
    /* Static cache of discovered terms: maximum size 20 000 sufficient for
     * n ≈ 10 000.  No dynamic allocation is used, keeping the code valid
     * for both C and C++ builds with arbitrary‑precision seq_t. */
    static seq_t cache[20000];
    static size_t cache_len = 0;
    static seq_t next_candidate = (seq_t)1;

    if (n == 0) return (seq_t)0;            /* undefined for offset 0 */
    while (cache_len < (size_t)n) {
        if (zeckendorf_len(next_candidate) == 8) {
            cache[cache_len++] = next_candidate;
        }
        ++next_candidate;
    }
    return cache[(size_t)n - 1];
}
