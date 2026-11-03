/*
 * A044170  Numbers k such that string 4,4 occurs in the base 7 representation
 *          of k but not of k-1.
 * https://oeis.org/A044170
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth integer k such that base‑7 representation of k *
 *            contains the substring "44" while base‑7 representation of k-1
 *            does not
 * Range:     u8 n <= 5, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Validated: partial, pass, 41 terms through n = 41 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Return true if the base‑7 representation of v contains two consecutive
 * digits 4. */
static bool contains_44(seq_t v)
{
    seq_t tmp = v;
    int prev = -1;                    /* previous digit, -1 indicates none yet */
    while (tmp > (seq_t)0)
    {
        int d = (int)(tmp % (seq_t)7);    /* current least‑significant digit */
        if (d == 4 && prev == 4)
            return true;                 /* found “44” */
        prev = d;
        tmp /= (seq_t)7;
    }
    return false;                       /* no “44” in this number */
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;        /* store previously computed terms */
    static size_t cache_len = 0;       /* number of terms stored */
    static size_t cache_cap = 0;       /* current allocation size     */
    static seq_t next_k = 32;          /* first term of the sequence  */

    if (n == 0)
        return 0;                       /* undefined for n = 0 */

    /* Grow the cache to contain at least n terms. */
    while (cache_len < (size_t)n)
    {
        /* Advance next_k until it satisfies the definition:
           it contains “44” and the previous number does not. */
        while (!(contains_44(next_k) && !contains_44(next_k - 1)))
            ++next_k;

        /* Resize the cache if necessary. */
        if (cache_len == cache_cap)
        {
            cache_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
            cache = mem_resize(seq_t, cache, cache_cap);
        }

        /* Store the term and prepare for the next search. */
        cache[cache_len++] = next_k;
        ++next_k;
    }

    return cache[n - 1];
}
