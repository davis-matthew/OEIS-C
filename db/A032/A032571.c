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
 * Helpers:   A032571_candidate_is_valid
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
#include "../utils/A032571_candidate_is_valid.h"

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
    seq_t *tmp = (seq_t *)realloc(A032571_cache, new_size * sizeof(seq_t));
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
