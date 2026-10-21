/*
 * A093505  a(n) = floor(A001969(n)/2 + 1/2).
 * https://oeis.org/A093505
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (A001969(n)+1)/2, where A001969(n) is the n‑th evil
 *            number (nonnegative integer with an even number of 1‑bits)
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   popcnt_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/popcnt_seq.h"

/* Compute the n‑th term of A093505.  idx_t is unsigned 64‑bit,
   seq_t is an unsigned integer type (8,16,32,64, or 128 bits),
   or an arbitrary‑precision integer class. */
seq_t a(idx_t n)
{
    /* static storage for already generated evil numbers
       (1‑based; evens[1] == 0). */
    static seq_t *evens = NULL;   /* array of evil numbers */
    static idx_t  len   = 0;      /* highest index computed, 1‑based */
    static size_t cap   = 0;      /* allocated array capacity */

    /* ------------------------------------------------------------------
       Initialise storage with the first evil number, 0.
       ------------------------------------------------------------------ */
    if (len == 0) {
        cap   = 16;
        evens = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!evens) return 0;                 /* allocation failure */
        evens[0] = 0;                         /* unused 0‑based slot */
        evens[1] = 0;                         /* A001969(1) = 0, a(1)=0 */
        len   = 1;
    }

    /* ------------------------------------------------------------------
       Extend the table until we have the n‑th evil number.
       ------------------------------------------------------------------ */
    for (idx_t i = len + 1; i <= n; ++i) {
        seq_t x = evens[len] + 1;            /* next integer to test */
        while (popcnt_seq(x) & 1) ++x;        /* skip odd‑popcount numbers */

        /* grow array if necessary */
        if (i >= cap) {
            size_t new_cap = cap * 2;
            seq_t *tmp = (seq_t *)realloc(evens, new_cap * sizeof(seq_t));
            if (!tmp) return 0;               /* allocation failure */
            evens = tmp;
            cap   = new_cap;
        }
        evens[i] = x;
        len = i;
    }

    /* ------------------------------------------------------------------
       Return the desired term: a(n) = (A001969(n)+1)/2.
       ------------------------------------------------------------------ */
    return (evens[n] + (seq_t)1) / (seq_t)2;
}
