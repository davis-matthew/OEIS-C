/*
 * A087119  Numbers having more than one maximal group of consecutive zeros in
 *          binary representation of n.
 * https://oeis.org/A087119
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes numbers having more than one maximal group of consecutive zeros
   in their binary representation (OEIS A087119).  The function searches
   for the property directly, building the sequence on demand.  */

#include <stddef.h>                  /* NULL, size_t */
#include <stdlib.h>                  /* malloc, realloc */
#include "../utils/bitlen_seq_t.h"

/* Count maximal runs of consecutive zeros in the binary representation
   of x.  The most significant bit of a positive integer is always 1,
   so we start with a preceding 1 to detect the first run. */
static int zero_runs(seq_t x)
{
    if (x == (seq_t)0) return 0;                 /* not used for positive x */

    unsigned int bits = bitlen_seq_t(x);          /* number of bits */
    int runs   = 0;
    int prev   = 1;                               /* pretend leading 1 */

    for (int i = (int)bits - 1; i >= 0; --i) {
        int bit = (int)((x >> i) & 1);
        if (bit == 0 && prev == 1)                /* start of a zero run */
            ++runs;
        prev = bit;
    }
    return runs;
}

/* Return the n-th term of the sequence (1‑based).  */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;                   /* undefined for n < 1 */

    static seq_t *seq   = NULL;                   /* cached terms      */
    static size_t seq_len = 0;                    /* number cached     */
    static size_t seq_cap = 0;                    /* allocated size    */
    static seq_t last_val = 0;                    /* last term cached  */

    /* Allocate or expand the cache. */
    if (!seq) {
        seq_cap = 64;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
    } else if (seq_len >= seq_cap) {
        seq_cap <<= 1;
        seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
    }

    /* Generate terms until we have at least n of them. */
    while ((idx_t)seq_len < n) {
        seq_t x = (seq_len == 0) ? (seq_t)1 : last_val + (seq_t)1;
        for (;; ++x) {
            if (zero_runs(x) >= 2) {
                seq[seq_len++] = x;
                last_val = x;
                break;
            }
        }
    }

    return seq[n - 1];
}
