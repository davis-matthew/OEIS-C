/*
 * A078943  a(1)=1; a(n+1) is either a(n)-n or a(n)+n, where we choose the
 *          smallest positive integer that's not among the values a(1), ...,
 *          a(n).
 * https://oeis.org/A078943
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 24, u32 n <= 24, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Generates OEIS A078943: a(1)=1; a(n+1) is either a(n)-n or a(n)+n,
   chosen as the smallest positive integer not already in the sequence. */

#include <stdlib.h>
#include <stdint.h>

/* The types seq_t and idx_t are provided by the build system.  No other
   globals may be added outside of this function. */
static seq_t *cache     = NULL;          /* sequence terms   */
static size_t cache_len = 0;             /* number of terms stored   */
static size_t cache_cap = 0;             /* current allocation size  */

/* Return the n‑th term of the sequence; if the sequence terminates
   before n, the function returns 0. */
seq_t a(idx_t n)
{
    /* n must be >= 1; for n==0 we return 0 */
    if (n == (idx_t)0) return (seq_t)0;

    /* initialise the sequence with the first term */
    if (cache_len == 0) {
        cache_cap = 4;
        cache     = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* allocation failure – useful for tests */
        cache[0]   = (seq_t)1;         /* a(1)=1 */
        cache_len  = 1;
    }

    /* grow the cache until we reach the requested index, or until a
       dead end occurs (no valid next term). */
    while (cache_len < n) {
        idx_t      pos  = cache_len;        /* current index (1‑based) */
        seq_t      cur  = cache[cache_len - 1];
        seq_t      cand_plus  = cur + (seq_t)pos;   /* a(n)+n   */
        seq_t      cand_minus = 0;
        int        have_minus = 0;

        if (cur > (seq_t)pos) {             /* a(n)-n is positive? */
            cand_minus = cur - (seq_t)pos;
            have_minus = 1;
        }

        /* find the smallest candidate not already used */
        seq_t next = 0;
        int   found = 0;

        /* search the existing terms for membership; linear scan is
           O(n) per step and is acceptable for n≤10000. */
        int used_plus  = 0, used_minus = 0;
        for (size_t i = 0; i < cache_len; ++i) {
            if (cache[i] == cand_plus)  used_plus = 1;
            if (have_minus && cache[i] == cand_minus) used_minus = 1;
            if (used_plus && (!have_minus || used_minus)) break;
        }

        if (have_minus && !used_minus) {
            if (!used_plus || cand_minus < cand_plus) {
                next  = cand_minus;
                found = 1;
            }
        }
        if (!found && !used_plus) {
            next  = cand_plus;
            found = 1;
        }

        if (!found) break;          /* dead end – sequence terminates */

        /* ensure enough space for the new term */
        if (cache_len >= cache_cap) {
            size_t new_cap = cache_cap * 2;
            seq_t *p = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!p) return (seq_t)0;   /* allocation failure */
            cache     = p;
            cache_cap = new_cap;
        }

        cache[cache_len] = next;
        cache_len++;
    }

    /* return requested term if it exists; otherwise 0 */
    if (cache_len >= n)
        return cache[n - 1];
    return (seq_t)0;
}
