/*
 * A240603  Numbers that are the sum of two successive squarefree numbers.
 * https://oeis.org/A240603
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A005117(n) + A005117(n+1) where A005117(k) is the k‑th
 *            squarefree integer
 * Range:     u8 n <= 77, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: this implementation builds the list of squarefree numbers up to
   the required index on demand.  It works for any seq_t that supports
   the usual arithmetic operations. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Helper: test whether an integer x (idx_t) is squarefree.
   Only integer arithmetic is used; x is small enough for 64‑bit
   arithmetic (up to about 20000 for n ≤ 10 000). */
static bool is_squarefree(idx_t x)
{
    for (idx_t p = 2; p * p <= x; ++p) {
        idx_t sq = p * p;
        if (x % sq == 0) return false;
    }
    return true;
}

/* Static cache of squarefree numbers: 0‑based indices.
   sq_len = number of elements stored (always ≥ 1). */
static seq_t *sq      = NULL;
static size_t sq_len  = 0;

/* Ensure that the cache holds squarefree numbers at least up to index
   `need` (0‑based).  `need` is the largest index required by the
   caller.  */
static void ensure_squarefree(idx_t need)
{
    /* Bail if already large enough. */
    if (sq_len > (size_t)need) return;

    /* Allocate new array: need+2 entries are safe (extra for growth). */
    size_t new_cap     = (size_t)need + 2;
    seq_t *new_arr     = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_arr) { abort(); }

    /* Build the list of squarefree numbers. */
    idx_t x     = 1;
    size_t pos  = 0;
    while (pos <= (size_t)need) {
        if (is_squarefree(x)) {
            new_arr[pos++] = (seq_t)x;
        }
        ++x;
    }

    /* Replace old cache.  The old cache is freed if non‑null. */
    if (sq) free(sq);
    sq      = new_arr;
    sq_len  = (size_t)need + 1;     /* now contains indices 0 … need */
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safe guard; sequence is defined for n ≥ 1 */

    /* We need the (n‑1)‑th and n‑th squarefree numbers (0‑based indices). */
    ensure_squarefree(n);

    return sq[(size_t)(n - 1)] + sq[(size_t)n];
}
