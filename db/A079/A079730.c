/*
 * A079730  Kolakoski variation using (1,2,3,4) starting with 1,2.
 * https://oeis.org/A079730
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A079730: Kolakoski variation over alphabet {1,2,3,4} starting with 1,2.
   The sequence is defined by a self‑referential run‑length construction:
   the run lengths of the infinite word are the sequence itself, and the
   run values cycle through 1,2,3,4.  This implementation builds the
   sequence incrementally, caching previously computed values for efficient
   random access. */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* cache for already computed terms (1‑based indexing) */
    static seq_t  *seq      = NULL;
    static size_t seq_cap   = 0;   /* allocated capacity (index 0 unused) */
    static size_t seq_len   = 0;   /* maximum index populated so far */
    static size_t run_i     = 0;   /* index into seq for the next run length */
    static idx_t  val       = 0;   /* current run value (cycles 1→4) */
    static size_t next_out  = 0;   /* next position to write in seq */

    /* initialise on the first call */
    if (seq == NULL) {
        /* initial capacity */
        seq_cap = 16;
        seq = (seq_t *)malloc((seq_cap + 1) * sizeof(seq_t));   /* index 0 unused */
        if (!seq) return 0; /* out of memory: return 0 (undefined for this code) */

        seq[1] = 1;
        seq[2] = 2;
        seq_len     = 2;
        run_i       = 1;
        val         = 1;
        next_out    = 1;
    }

    /* extend the cache until the requested index is available */
    while (seq_len < (size_t)n) {
        /* current run length from the already known sequence */
        size_t len = (size_t)seq[run_i];

        /* ensure capacity for the new terms */
        size_t needed = next_out + len - 1;
        if (needed > seq_cap) {
            size_t new_cap = seq_cap ? seq_cap * 2 : 16;
            while (new_cap < needed) new_cap *= 2;
            seq = (seq_t *)realloc(seq, (new_cap + 1) * sizeof(seq_t));
            seq_cap = new_cap;
        }

        /* write the run of length 'len' with value 'val' */
        for (size_t k = 0; k < len; ++k) {
            seq[next_out++] = (seq_t)val;
        }
        seq_len = next_out - 1;

        /* advance to the next run */
        ++run_i;
        ++val;
        if (val > 4) val = 1;
    }

    return seq[n];
}
