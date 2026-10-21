/*
 * A080607  Golomb's sequence using multiples of 3.
 * https://oeis.org/A080607
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   The sequence is generated block by block. The k‑th block (k≥1)
 *            consists of the value 3·k repeated a(k) times, where a(k) is the
 *            k‑th term of the sequence itself (a(1)=3)
 * Range:     u8 n <= 1980, u16 n <= 7368, u32 n <= 7368, u64 n <= 7368,
 *            u128 n <= 7368, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Requires: seq_t and idx_t types supplied by the build system.
   No external helpers are used. */

#include <stdlib.h>

/* Return the n‑th term of OEIS A080607 (1‑based). */
seq_t a(idx_t n)
{
    /* n == 0 is outside the defined offset; return 0. */
    if (n == 0)
        return (seq_t)0;

    /* Static state to keep previously computed terms. */
    static seq_t *seq = NULL;      /* array of a(i) values */
    static size_t sz  = 0;         /* number of values available */
    static size_t cap = 0;         /* capacity of the array */
    static idx_t  next_block = 1;  /* block number to generate next */

    /* Ensure we have generated enough terms to answer the request. */
    while (sz < (size_t)n) {
        seq_t len;          /* length of the next block (a(k)) */
        seq_t val;          /* value to write into the block (3·k) */

        /* Determine the length of this block. */
        if (next_block == 1) {
            /* a(1) is defined to be 3. */
            len = (seq_t)3;
        } else {
            /* For k ≥ 2, a(k) is the k‑th term already stored. */
            /* k-1 is zero‑based index into seq. */
            if ((size_t)(next_block - 1) >= sz) {
                /* This situation should not happen: we must have
                   computed a(k) before attempting to use it. */
                /* As a safety fallback, set len to 0 to avoid infinite loop. */
                len = 0;
            } else {
                len = seq[next_block - 1];
            }
        }

        /* Value of the k‑th block. */
        val = (seq_t)(next_block * 3);

        /* Allocate enough capacity for the new entries. */
        size_t needed = sz + (size_t)len;
        if (needed > cap) {
            size_t newcap = cap ? cap * 2 : 64;
            while (newcap < needed)
                newcap *= 2;
            seq = (seq_t *)realloc(seq, newcap * sizeof(seq_t));
            if (!seq) {  /* allocation failed */
                return (seq_t)0;
            }
            cap = newcap;
        }

        /* Append the entire block; we must generate all entries
           even if the requested n lies inside this block. */
        for (size_t i = 0; i < (size_t)len; ++i)
            seq[sz++] = val;

        /* Move to next block number. */
        ++next_block;
    }

    /* Now seq[n‑1] holds a(n). */
    return seq[n - 1];
}
