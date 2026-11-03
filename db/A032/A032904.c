/*
 * A032904  Numbers whose base-7 representation Sum_{i=0..m} d(i)*7^i has d(0)
 *          <= d(1) >= d(2) <= ...
 * https://oeis.org/A032904
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Validated: partial, pass, 69 terms through n = 69 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A032904: numbers whose base‑7 digits satisfy
   d0 <= d1 >= d2 <= d3 ... */
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Return true if x has the required alternating pattern in base 7. */
static bool has_alternating_pattern(seq_t x)
{
    if (x == 0) return false;            /* 0 is not in the sequence */
    seq_t t = x;
    int prev = (int)(t % 7);           /* least significant digit   */
    t /= 7;
    int pos = 1;                        /* next digit is position 1 (odd) */

    while (t > 0) {
        int cur = (int)(t % 7);
        if (pos & 1) {                  /* odd position: prev <= cur   */
            if (prev > cur) return false;
        } else {                       /* even position: prev >= cur */
            if (prev < cur) return false;
        }
        prev = cur;
        t /= 7;
        ++pos;
    }
    return true;
}

/* Grow the array (*arr) to hold at least 'needed' elements. */
static void grow_array(seq_t **arr, size_t *cap, size_t needed)
{
    if (*cap >= needed) return;
    size_t newcap = (*cap) ? (*cap) << 1 : 16;
    while (newcap < needed) newcap <<= 1;
    seq_t *newptr = mem_resize(seq_t, *arr, (newcap * sizeof(**arr)) / sizeof(seq_t));
    *arr = newptr;
    *cap = newcap;
}

/* Return the nth term (n >= 1) of the sequence. */
seq_t a(idx_t n)
{
    static seq_t *terms = NULL;   /* cached terms */
    static size_t cap   = 0;      /* capacity of 'terms'   */
    static size_t sz    = 0;      /* number of terms stored*/
    static seq_t last   = 0;      /* last integer examined  */

    /* Undefined for n == 0; return 0 (unused). */
    if (n == 0) return 0;

    /* Grow cache as needed. */
    grow_array(&terms, &cap, (size_t)n);

    /* Generate terms until we have at least n. */
    while (sz < (size_t)n) {
        ++last;
        if (has_alternating_pattern(last)) {
            terms[sz++] = last;
        }
    }
    return terms[(size_t)n - 1];
}
