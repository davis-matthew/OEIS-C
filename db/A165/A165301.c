/*
 * A165301  a(n) is the smallest number not already in the sequence, such that
 *          the concatenation of all a(n) displays the periodic digit string
 *          1, 2, 3 (and repeat).
 * https://oeis.org/A165301
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 30, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A165301.
   The sequence is defined recursively: a(1) = 1, a(2) = 2, a(3) = 3.
   For n > 3, let S be the infinite periodic digit string 123123….
   Let p be the number of digits already written by concatenating a(1)..a(n‑1).
   a(n) is the smallest unused natural number whose decimal representation
   equals the substring of S starting at position p.  This is computed
   greedily: extend the substring character by character until a number
   not in the sequence is found.  The first such number is also the
   smallest possible. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Global dynamic arrays for memoisation --------------------------------- */
static seq_t *seq = NULL;          /* sequence values */
static size_t seq_cap = 0, seq_len = 0;
static seq_t *used = NULL;         /* list of used numbers */
static size_t used_cap = 0, used_len = 0;
static idx_t cur_pos = 0;          /* current position in the 123… string */

/* Linear membership test.  The sequence is small enough that its
   cost is negligible.  */
static bool is_used(seq_t v)
{
    for (size_t i = 0; i < used_len; ++i)
        if (used[i] == v) return true;
    return false;
}

/* Append a value to the used list, resizing if necessary. */
static void used_add(seq_t v)
{
    if (used_len == used_cap) {
        size_t new_cap = used_cap ? used_cap * 2 : 8;
        seq_t *tmp = (seq_t *)realloc(used, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        used = tmp;
        used_cap = new_cap;
    }
    used[used_len++] = v;
}

/* Append a value to the sequence array, resizing if necessary. */
static void seq_add(seq_t v)
{
    if (seq_len == seq_cap) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 8;
        seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        seq = tmp;
        seq_cap = new_cap;
    }
    seq[seq_len++] = v;
}

/* Main function: compute a(n) for any n >= 1, ignoring n == 0.  */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* safety: out‑of‑range */

    size_t target = (size_t)n - 1;  /* 0‑based index */

    /* Grow the sequence up to the requested index. */
    while (seq_len <= target) {
        /* Find the smallest unused number that matches the next
           substring of 123123….  */
        unsigned __int128 cand128 = 0;
        idx_t   cand_len = 0;
        seq_t   chosen = 0;
        bool    found = false;

        /* Maximum value representable by seq_t. */
        seq_t max_val = (seq_t)-1;

        for (idx_t len = 1; !found; ++len) {
            /* Digit at position cur_pos + (len-1) in the periodic string. */
            idx_t pos = cur_pos + (len - 1);
            unsigned int digit = (unsigned int)(pos % 3) + 1;   /* 1,2,3 cyclic */

            cand128 = cand128 * 10 + digit;
            if (cand128 > (unsigned __int128)max_val) break;   /* cannot fit */

            seq_t cand = (seq_t)cand128;
            if (!is_used(cand)) {
                chosen = cand;
                cand_len = len;
                found = true;
            }
        }

        /* Append the found term.  This should always succeed for small
           seq_t sizes until the maximum number becomes unreachable; in
           that case the algorithm stops.  */
        seq_add(chosen);
        used_add(chosen);
        cur_pos += cand_len;
    }

    return seq[target];
}
