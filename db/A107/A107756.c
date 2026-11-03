/*
 * A107756  Numbers k such that Sum_{j=1..k} Catalan(j) == 1 (mod 3).
 * https://oeis.org/A107756
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Validated: partial, pass, 73 terms through n = 73 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A107756
    Sequence: numbers k such that the base‑3 expansion of k+1 contains a 2.
    The algorithm simply iterates over k and keeps the ones that satisfy this
    property.  Results are cached so that subsequent calls are O(1) for
    already computed indices. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Return true iff the base‑3 representation of (n+1) contains the digit 2. */
static bool has_digit2(idx_t n)
{
    idx_t x = n + 1;               /* look at n+1 */
    while (x != 0) {
        if (x % 3 == 2)
            return true;
        x /= 3;
    }
    return false;
}

/* static helpers ---------------------------------------------------------- */

/* Ensure that array has room for at least 'need' elements.  */
static void ensure_capacity(idx_t need)
{
    static idx_t capacity = 0;
    static seq_t *array   = NULL;
    if (capacity < need) {
        idx_t new_cap = capacity ? (capacity << 1) : 1;
        while (new_cap < need)
            new_cap <<= 1;
        array = mem_resize(seq_t, array, new_cap);
        capacity = new_cap;
    }
    /* We attach array and capacity to the outer function's static variables. */
}

/*  Public function -------------------------------------------------------- */

/* Return the n-th term (1‑based) of the sequence.  */
seq_t a(idx_t n)
{
    static seq_t *seq   = NULL;
    static idx_t  cap   = 0;
    static idx_t  sz    = 0;
    static idx_t  next  = 1;          /* candidate k to test next */

    /* Allocate or expand the array as needed. */
    while (sz < n) {
        idx_t k = next++;

        if (has_digit2(k)) {
            if (cap < sz + 1) {
                idx_t new_cap = cap ? (cap << 1) : 1;
                while (new_cap < sz + 1)
                    new_cap <<= 1;
                seq = mem_resize(seq_t, seq, new_cap);
                cap = new_cap;
            }
            seq[sz++] = (seq_t)k;
        }
    }

    return seq[n - 1];
}
