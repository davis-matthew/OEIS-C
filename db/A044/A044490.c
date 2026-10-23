/*
 * A044490  Numbers n such that string 1,1 occurs in the base 6 representation
 *          of n but not of n+1.
 * https://oeis.org/A044490
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   has_substring_11
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements a(n) by brute‑force search.  For each integer k ≥ 1,
   we test whether the base‑6 representation of k contains the
   substring “11” and the base‑6 representation of k+1 does not.
   The n‑th such k (the sequence A044490) is returned. */

#include <stdbool.h>
#include "../utils/has_substring_11.h"

/* OEIS A044490 – numbers n whose base‑6 representation contains the
   substring “11” but that of n+1 does not. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety for out‑of‑range call */

    idx_t count = 0;
    idx_t k = 1;                   /* current candidate (1‑based) */
    seq_t answer = 0;

    while (count < n) {
        seq_t cand = (seq_t)k;      /* candidate n */
        if (has_substring_11(cand) && !has_substring_11(cand + 1)) {
            count++;
            if (count == n) {
                answer = cand;
                break;
            }
        }
        k++;                        /* next integer to test */
    }
    return answer;
}
