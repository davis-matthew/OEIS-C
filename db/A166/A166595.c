/*
 * A166595  The number of vertices of a hypercube modulo the number of
 *          largest-dimensional surface tiles, in the first fourteen cases of
 *          the residue being greater than the dimension number.
 * https://oeis.org/A166595
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, uned
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 14, u32 n <= 14, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   alloc_seq, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) is the n-th residue r = 2^d mod (2d) for which r > d.
   The dimensions d are scanned in increasing order.  Each time a residue
   satisfies r > d it is stored; the n-th such residue is returned. */
#include <stdlib.h>
#include "../utils/mod_pow.h"
#include "../utils/alloc_seq.h"

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;      /* not defined below offset */
    /* Static state for memoisation */
    static idx_t next_d = 1;          /* next dimension to test */
    static seq_t *values = NULL;      /* array of found residues */
    static size_t capacity = 0;       /* allocated capacity of values */
    static size_t count = 0;          /* number of residues found so far */

    /* Grow the array on first use */
    if (!values) {
        capacity = 64;
        values = alloc_seq(capacity);    /* alloc_seq returns seq_t* */
    }

    /* Compute until we have at least n residues */
    while (count < n) {
        seq_t mod = (seq_t)2 * (seq_t)next_d;
        seq_t r   = mod_pow((seq_t)2, next_d, mod);

        if (r > next_d) { /* condition r > d  */
            /* Resize if necessary */
            if (count == capacity) {
                capacity <<= 1;            /* double capacity */
                values = (seq_t *)realloc(values,
                                            capacity * sizeof(seq_t));
            }
            values[count++] = r;
        }
        ++next_d;
    }

    return values[n - 1];
}
