/*
 * A072482  Partition the nonnegative integers into minimal groups whose sums
 *          are palindromes; this sequence gives the sums.
 * https://oeis.org/A072482
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = sum of the minimal block of consecutive nonnegative
 *            integers starting after the previous block whose sum is a
 *            decimal palindrome. The blocks are formed greedily: for each
 *            block, add the next integer until the running sum is a
 *            palindrome, then the block ends and the sum is a term
 * Range:     u8 n <= 11, u16 n <= 34, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Build‑time typedefs are provided: seq_t (unsigned or arbitrary‑precision integer),
   idx_t (typically uint64_t).  Scan the sequence by caching computed terms. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* -------------------------------------------------------------- */
/* Test whether a seq_t value is a decimal palindrome.            */
/* -------------------------------------------------------------- */
static bool is_pal(seq_t x)
{
    /* 0 is a palindrome */
    if (x == 0)
        return true;

    seq_t rev = 0;
    seq_t tmp = x;
    while (tmp > 0) {
        rev = rev * 10 + (tmp % 10);
        tmp = tmp / 10;
    }
    return rev == x;
}

/* -------------------------------------------------------------- */
/* Cached storage for computed sequence values.                   */
/* -------------------------------------------------------------- */
static seq_t *cached_terms = NULL;   /* a(0), a(1), … */
static idx_t  cached_len   = 0;      /* number of cached terms        */
static idx_t  cached_cap   = 0;      /* allocated capacity            */
static idx_t  next_start   = 0;      /* first unused integer for next block */

/* -------------------------------------------------------------- */
/* Ensure that a(0) … a(upto) are computed and cached.           */
/* -------------------------------------------------------------- */
static void ensure_terms_cached(idx_t upto)
{
    while (cached_len <= upto) {
        seq_t sum = 0;
        for (idx_t i = next_start; ; ++i) {
            sum += (seq_t)i;
            if (is_pal(sum)) {
                /* Grow the cache if necessary. */
                if (cached_len == cached_cap) {
                    if (cached_cap == 0)
                        cached_cap = 64;
                    else
                        cached_cap <<= 1;
                    cached_terms = (seq_t*)realloc(cached_terms,
                                                   cached_cap * sizeof(seq_t));
                }
                cached_terms[cached_len++] = sum;
                next_start = i + 1;
                break;
            }
        }
    }
}

/* -------------------------------------------------------------- */
/* Public entry point: the n-th term of A072482.                  */
/* -------------------------------------------------------------- */
seq_t a(idx_t n)
{
    ensure_terms_cached(n);
    return cached_terms[n];
}
