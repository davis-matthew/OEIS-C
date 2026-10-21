/*
 * A063950  Of course every number is the sum of 4 squares; these are the
 *          numbers not of the form 4m+2 such that the first square can be
 *          taken to be any positive square < n.
 * https://oeis.org/A063950
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 47, u16 n <= 109, u32 n <= 109, u64 n <= 109,
 *            u128 n <= 109, big any n
 * Helpers:   good_value, three_square
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A063950
 *  Definition: a(n) is the n‑th integer not congruent to 2 modulo 4 such that
 *  for every positive integer a with 1 <= a^2 < a(n), the difference
 *  a(n) - a^2 is not of the form 4^k (8m + 7).  This is equivalent to
 *  requiring that for each such a the number a(n) can be expressed as
 *  a^2 + b^2 + c^2 + d^2 with non‑negative integers b, c, d.
 *  (The classical three‑square theorem guarantees that k is representable
 *  as a sum of three squares iff it is not of the form 4^k(8m+7).)
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/good_value.h"
#include "../utils/three_square.h"

/* dynamic array to memoise the sequence */
static seq_t *memo      = NULL;
static size_t memo_len  = 0;
static size_t memo_cap  = 0;

/* ensure the memo array can hold at least 'need' elements */
static void ensure_capacity(size_t need)
{
    if (memo_cap >= need) return;
    size_t new_cap = memo_cap ? memo_cap * 2 : 16;
    while (new_cap < need) new_cap <<= 1;
    memo = (seq_t *)realloc(memo, new_cap * sizeof(seq_t));
    memo_cap = new_cap;
}

/* the required function */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* offset is 1: a(1) is the first term */

    /* extend memoisation until we have at least 'n' terms */
    while (memo_len < n)
    {
        idx_t candidate =
            (memo_len == 0) ? 0 : (idx_t)memo[memo_len - 1] + 1;

        while (!good_value(candidate)) ++candidate;

        ensure_capacity(memo_len + 1);
        memo[memo_len++] = (seq_t)candidate;
    }

    return memo[n - 1];
}
