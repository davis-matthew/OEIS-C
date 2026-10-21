/*
 * A080944  Numbers having only trivial divisors that are also suffixes in
 *          binary representation.
 * https://oeis.org/A080944
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 200, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence consists of the positive integers n for which every
 * non‑zero proper binary suffix d of n is either a power of two or
 * does not divide n.  The function returns the n‑th element
 * (1‑based).  It memoises the values so that calls in any order
 * are O(1) amortised after the early terms are generated.  */
 
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

/* Compute the binary length of x (>0).  For x==0 returns 0. */
static inline size_t bitlen_seq_t(seq_t x)
{
    size_t len = 0;
    while (x) {
        x >>= 1;
        ++len;
    }
    return len;
}

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache       = NULL;   /* stored terms         */
    static size_t cache_len  = 0;       /* current number of terms   */
    static size_t cache_cap  = 0;       /* allocated capacity       */
    static seq_t next_candidate = 1;   /* next integer to test     */

    /* undefined for n < 1; return 0 safely. */
    if (n == 0) return 0;

    /* generate terms until we have at least n of them */
    while (cache_len < n) {
        seq_t x = next_candidate;
        size_t bits = bitlen_seq_t(x);

        bool good = true;
        /* test every proper suffix */
        for (size_t l = 1; l < bits && good; ++l) {
            seq_t mask = ((seq_t)1 << l) - 1;
            seq_t d = x & mask;
            if (d == 0) continue;               /* zero suffix ignored */
            if ((d & (d - 1)) == 0) continue;   /* d is a power of two */
            if (x % d == 0) good = false;        /* non‑power‑two divisor found */
        }

        if (good) {
            /* ensure sufficient capacity */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap == 0 ? 128 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) exit(1);               /* allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = x;
        }
        ++next_candidate;
    }

    return cache[n - 1];
}
