/*
 * A032571  Numbers k whose decimal representation, read as a base-20 value
 *          and divided by k, yields an integer.
 * https://oeis.org/A032571
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Validated: partial, pass, 49 terms through n = 49 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n‑th term of OEIS sequence A032571.
   A032571 consists of the integers k whose decimal representation,
   interpreted as a base‑20 number, is divisible by k.
   The function builds the list by brute‑force search, memoising
   previously found terms. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Test whether candidate satisfies: (decimal digits as base‑20) % candidate == 0 */
static bool
A032571_candidate_is_valid(idx_t cand)
{
    if (cand == 0) return false;
    unsigned __int128 val = 0;          /* current accumulated value modulo cand */
    unsigned __int128 pw  = 1;          /* 20^i modulo cand           */
    idx_t temp = cand;
    while (temp > 0) {
        unsigned digit = temp % 10;
        val = (val + (unsigned __int128)digit * pw) % cand;
        pw  = (pw * 20) % cand;
        temp /= 10;
    }
    return val == 0;
}

/* Cache of computed terms */
static seq_t *A032571_cache = NULL;
static size_t A032571_cap   = 0;   /* allocated capacity */
static idx_t  A032571_size  = 0;   /* number of stored terms   */
static idx_t  A032571_next  = 1;   /* next integer to test    */

/* Ensure the cache has at least 'new_cap' elements of space */
static void
A032571_ensure_capacity(size_t new_cap)
{
    if (A032571_cap >= new_cap) return;
    size_t new_size = (A032571_cap == 0) ? 256 : A032571_cap * 2;
    if (new_size < new_cap) new_size = new_cap;
    seq_t *tmp = mem_resize(seq_t, A032571_cache, new_size);
    A032571_cache = tmp;
    A032571_cap   = new_size;
}

/* Return the nth term of the sequence (1‑based indexing) */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* behaviour below the offset is undefined */

    while (A032571_size < n) {
        if (A032571_candidate_is_valid(A032571_next)) {
            A032571_ensure_capacity(A032571_size + 1);
            A032571_cache[A032571_size++] = (seq_t)A032571_next;
        }
        ++A032571_next;
    }
    return A032571_cache[n - 1];
}
