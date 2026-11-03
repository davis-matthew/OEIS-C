/*
 * A044693  Numbers n such that string 6,7 occurs in the base 9 representation
 *          of n but not of n+1.
 * https://oeis.org/A044693
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   brute‑force search for numbers whose base‑9 representation *
 *            contains the substring "67" but whose successor n+1 does NOT
 *            contain it
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Validated: partial, pass, 39 terms through n = 39 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Utility: check whether base‑9 representation of x contains "67" */
static bool contains67(seq_t x)
{
    if (x == 0) return false;                 /* base‑9 of 0 is "0" */
    seq_t digits[128];
    int len = 0;
    while (x > 0) {
        digits[len++] = x % 9;
        x /= 9;
    }
    /* digits[0] is least significant, digits[len-1] most significant.
       Scan from most significant downwards. */
    for (int i = len - 1; i > 0; --i) {
        if (digits[i] == 6 && digits[i - 1] == 7)
            return true;
    }
    return false;
}

/* Main function required by the test harness */
seq_t a(idx_t n)
{
    static seq_t *terms = NULL;      /* array of found terms   */
    static idx_t  size  = 0;         /* current number of terms */
    static idx_t  cap   = 0;         /* capacity of terms array */
    static seq_t  next  = 1;         /* next candidate to test */

    /* Grow terms array if needed */
    if (size == 0 || size == cap) {
        cap = (cap > 0) ? 2 * cap : 16;
        terms = mem_resize(seq_t, terms, cap);
        /* In practice the allocation will not fail for the ranges
           we need to support. */
    }

    /* Generate terms until we have at least n of them. */
    while (size < n) {
        /* Check condition for current candidate. */
        if (contains67(next) && !contains67(next + 1)) {
            terms[size++] = next;
        }
        ++next;
    }

    return terms[n - 1];
}
