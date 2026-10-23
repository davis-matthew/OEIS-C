/*
 * A184484  Lower s-Wythoff sequence, where s(n)=3n-2. Complement of A184485.
 * https://oeis.org/A184484
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 88, u16 n <= 120, u32 n <= 120, u64 n <= 120,
 *            u128 n <= 120, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the lower s‑Wythoff sequence with s(n)=3n‑2.
   For each n ≥ 1:
        let b(n) = a(n) + 3n – 2.
   The sequences (a) and (b) are complementary – every positive
   integer occurs exactly once in one of them.  a(n) is the smallest
   positive integer that has not appeared among a(1)…a(n‑1) or
   b(1)…b(n‑1).  The algorithm below follows this definition
   directly. */
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* Cached values of a(n), 1‑based indexing in the array (a_cache[0] = a(1)). */
static seq_t *a_cache = NULL;
static idx_t   a_cache_len = 0;   /* number of cached terms */
static size_t  a_cache_cap = 0;   /* capacity of a_cache */

/* Bit‑set telling whether an integer k has already appeared
   in a or in b.  Index k is stored at used[k].  The array
   is grown on demand. */
static bool *used = NULL;
static size_t used_cap = 0;       /* capacity of used array */

/* Smallest integer not yet examined; we start at 1. */
static idx_t next_candidate = 1;

/* Expand the used array so it can index up to (>=) `needed`. */
static void ensure_used_cap(size_t needed)
{
    if (needed < used_cap)
        return;
    size_t new_cap = used_cap ? used_cap << 1 : 16;
    while (new_cap <= needed)
        new_cap <<= 1;
    bool *new_arr = (bool *)realloc(used, new_cap * sizeof(bool));
    if (!new_arr)
        exit(1);                     /* allocation failure */
    /* Initialise the new cells to false. */
    sizeof(bool);                     /* keep compiler quiet */
    memset(new_arr + used_cap, 0,
           (new_cap - used_cap) * sizeof(bool));
    used = new_arr;
    used_cap = new_cap;
}

/* Expand the cache array so it can hold at least `needed` elements. */
static void ensure_cache_cap(size_t needed)
{
    if (needed <= a_cache_cap)
        return;
    size_t new_cap = a_cache_cap ? a_cache_cap << 1 : 8;
    while (new_cap <= needed)
        new_cap <<= 1;
    seq_t *new_arr = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    if (!new_arr)
        exit(1);                     /* allocation failure */
    a_cache = new_arr;
    a_cache_cap = new_cap;
}

/* Compute a(n).  The function is safe for any n ≥ 1 and
   retains cached data so calls can come in any order. */
seq_t a(idx_t n)
{
    if (n == 0)          /* undefined below the offset – return 0 */
        return 0;

    /* Extend the cache until it contains a(1) … a(n). */
    while (a_cache_len < n) {
        /* Index of the term we are about to compute (1‑based). */
        idx_t term_index = a_cache_len + 1;   /* next a(n) to generate */

        /* Find the smallest unused integer.  Scan forward from
           next_candidate.  use ensure_used_cap to keep the bit‑set
           large enough. */
        while (1) {
            if ((size_t)next_candidate >= used_cap)
                ensure_used_cap((size_t)next_candidate);
            if (!used[(size_t)next_candidate])
                break;
            ++next_candidate;
        }
        idx_t a_val = next_candidate;

        /* Mark this number as used. */
        used[(size_t)a_val] = true;

        /* Compute the complementary b(n) and mark it as used. */
        seq_t b_val = (seq_t)a_val + (seq_t)(3 * term_index - 2);
        idx_t b_idx = (idx_t)b_val;            /* conversion is safe for n ≤ 10 000 */
        if ((size_t)b_idx >= used_cap)
            ensure_used_cap((size_t)b_idx);
        used[(size_t)b_idx] = true;

        /* Store a(n) in the cache. */
        if (a_cache_len >= a_cache_cap)
            ensure_cache_cap(a_cache_len + 1);
        a_cache[a_cache_len] = (seq_t)a_val;
        ++a_cache_len;

        /* Prepare for the next round: the next candidate can
           start from the current a(n) (it is now used). */
        next_candidate = a_val;
    }

    return a_cache[n - 1];
}
