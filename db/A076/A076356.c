/*
 * A076356  Numbers n such that 210*n-11 is prime.
 * https://oeis.org/A076356
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 111, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 82 terms through n = 956 (2026-11-03)
 * Helpers:   is_prime, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A076356: n such that 210*n-11 is prime.
   The function returns the n-th such integer.
   Search method: test numbers in order and keep a dynamic table. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/mem.h"

/* Return the n-th term of the sequence.
   The value itself is the integer n for which 210*n−11 is prime. */
seq_t a(idx_t n)
{
    /* Static cache of found terms. */
    static seq_t *table = NULL;          /* array of terms */
    static idx_t len   = 0;              /* number of stored terms */
    static idx_t cap   = 0;              /* allocated capacity */
    static idx_t last_checked = 0;       /* last integer n we tested */

    /* Allocate the table once. */
    if (cap == 0) {
        cap = 4;
        table = mem_new(seq_t, cap);
    }

    /* Extend the table until we have at least n terms. */
    while (len < n) {
        /* Check the next candidate integer. */
        ++last_checked;

        /* 210*last_checked - 11 (performed in 64‑bit to avoid overflow). */
        uint64_t cand = 210ULL * (uint64_t)last_checked - 11ULL;

        /* Prime test via the provided 64‑bit routine. */
        if (is_prime((idx_t)cand)) {
            /* Grow the array if needed. */
            if (len == cap) {
                cap <<= 1;
                table = mem_resize(seq_t, table, cap);
            }
            table[len++] = last_checked;   /* store the integer itself */
        }
    }

    return table[n - 1];
}
