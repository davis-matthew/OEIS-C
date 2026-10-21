/*
 * A065337  a(n) is the smallest number not already used such that Sum_{m = 0
 *          .. n-1} a(m)*a(m+1) is a square.
 * https://oeis.org/A065337
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 13, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * This code implements a(n) from OEIS A065337:
 * a(0) = 1.
 * For n > 0, a(n) is the smallest positive integer not already used such that
 *    S(n) = Σ_{m=0}^{n-1} a(m)*a(m+1)   is a perfect square.
 *
 * The implementation memoizes previously computed terms and keeps the
 * running sum S(n) to avoid recomputing the whole sum at each step.
 * It is written to compile as both C11 and C++17; seq_t may be a native
 * integer type or an arbitrary‑precision integer class.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/floor_sqrt.h"

static seq_t *seq_cache   = NULL;   /* dynamic array of computed terms   */
static size_t seq_cap    = 0;       /* capacity of seq_cache             */
static idx_t  seq_len    = 0;       /* number of terms already computed */
static seq_t  sum_so_far = 0;       /* S(k) for the last computed index */

/* Grow seq_cache so that it can hold at least ‘need’ elements. */
static void
ensure_seq_cap(size_t need)
{
    if (need <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap : 16;
    while (new_cap < need) new_cap <<= 1;
    seq_t *newp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!newp) exit(1);
    seq_cache = newp;
    seq_cap   = new_cap;
}

/* Check whether ‘val’ has already been used in indices 0 .. upto-1. */
static bool
is_used(seq_t val, idx_t upto)
{
    for (idx_t j = 0; j < upto; ++j)
        if (seq_cache[j] == val) return true;
    return false;
}

/* Main sequence function. */
seq_t
a(idx_t n)
{
    /* Generate terms up to index n if they are not already computed. */
    if ((idx_t)seq_len <= n)
    {
        for (idx_t i = seq_len; i <= n; ++i)
        {
            ensure_seq_cap((size_t)i + 1);

            if (i == 0)
            {
                seq_cache[0] = (seq_t)1;   /* a(0) = 1                 */
                sum_so_far   = 0;          /* no products added yet     */
            }
            else
            {
                const seq_t prev = seq_cache[i - 1];
                seq_t cand = (seq_t)1;

                for (;; ++cand)
                {
                    if (is_used(cand, i)) continue;

                    const seq_t product   = prev * cand;
                    const seq_t new_sum   = sum_so_far + product;
                    const seq_t root      = floor_sqrt(new_sum);

                    if (root * root == new_sum)
                    {
                        seq_cache[i] = cand;
                        sum_so_far   = new_sum;
                        break;
                    }
                }
            }
        }
        seq_len = n + 1;
    }

    return seq_cache[(size_t)n];
}
