/*
 * A043975  Numbers k such that 2 and 3 occur juxtaposed in the base-9
 *          representation of k but not of k+1.
 * https://oeis.org/A043975
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Validated: partial, pass, 42 terms through n = 42 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Definition: a(n) = the n‑th integer k such that
 * the base‑9 representation of k contains the adjacent digits 2,3
 * or 3,2, but the base‑9 representation of k+1 does not. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Return true iff x in base‑9 contains the substring 23 or 32. */
static bool has_adjacent_23_32(seq_t x)
{
    if (x == (seq_t)0) return false;

    int prev = -1;
    while (x != (seq_t)0) {
        int d = (int)(x % 9);
        if (prev != -1) {
            if ((prev == 2 && d == 3) || (prev == 3 && d == 2))
                return true;
        }
        prev = d;
        x = x / 9;
    }
    return false;
}

/* Return the n‑th term of the sequence (n >= 1). */
seq_t a(idx_t n)
{
    /* Memoised terms.  Start with an initial capacity. */
    static seq_t *terms = NULL;
    static size_t terms_len = 0;
    static size_t terms_cap = 256;   /* initial capacity */
    static seq_t next_k = 1;         /* next integer to test */

    /* Allocate the cache lazily. */
    if (terms == NULL) {
        terms = mem_new(seq_t, terms_cap);
        if (terms == NULL) return (seq_t)0; /* overflow, unlikely */
    }

    /* Generate terms until we have at least n of them. */
    while (terms_len < (size_t)n) {
        seq_t k = next_k;
        if (has_adjacent_23_32(k) && !has_adjacent_23_32(k + (seq_t)1)) {
            /* Store k.  Resize if necessary. */
            if (terms_len == terms_cap) {
                size_t new_cap = terms_cap * 2;
                seq_t *tmp = mem_resize(seq_t, terms, new_cap);
                if (tmp == NULL) return (seq_t)0; /* overflow, unlikely */
                terms = tmp;
                terms_cap = new_cap;
            }
            terms[terms_len++] = k;
        }
        next_k = k + (seq_t)1;
    }

    return terms[n - 1];
}
