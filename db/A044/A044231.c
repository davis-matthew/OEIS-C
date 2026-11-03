/*
 * A044231  Numbers n such that string 5,4 occurs in the base 8 representation
 *          of n but not of n-1.
 * https://oeis.org/A044231
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth integer n such that the octal (base‑8)
 *            representation of n contains the substring "54" but the
 *            representation of n-1 does not
 * Range:     u8 n <= 4, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Validated: partial, pass, 41 terms through n = 41 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Check if the octal representation of x contains the substring 5,4.
   We scan from least significant digit to most significant digit and
   look for the reversed pair 4,5. */
static bool contains54(seq_t x)
{
    if (x == (seq_t)0) return false;
    seq_t tmp = x;
    int prev = -1;
    while (tmp > (seq_t)0) {
        int cur = (int)(tmp % (seq_t)8);
        if (prev == 4 && cur == 5) return true;
        prev = cur;
        tmp = tmp / (seq_t)8;
    }
    return false;
}

/* Cache for already computed terms. */
static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap = 0;
static seq_t last_checked = (seq_t)0;

/* Return the nth term (1‑based) of A044231.  */
seq_t a(idx_t n)
{
    /* Ensure enough terms are computed. */
    while (cache_size < n) {
        last_checked = last_checked + (seq_t)1;          /* next candidate */
        seq_t cand = last_checked;
        if (contains54(cand) && !contains54(cand - (seq_t)1)) {
            if (cache_size == cache_cap) {
                cache_cap = cache_cap ? cache_cap * 2 : 64;
                cache = mem_resize(seq_t, cache, cache_cap);
                if (!cache) exit(EXIT_FAILURE);
            }
            cache[cache_size++] = cand;
        }
    }
    return cache[n - 1];
}
