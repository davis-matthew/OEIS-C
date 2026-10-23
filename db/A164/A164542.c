/*
 * A164542  a(n) = 8*a(n-1) - 8*a(n-2) for n > 1; a(0) = 1, a(1) = 16.
 * https://oeis.org/A164542
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-1) - 8*a(n-2) (a(0)=1, a(1)=16)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Static buffer to memoise previously computed terms.
       `seq` holds the values; `len` is the number of values currently stored;
       `cap` is the allocated capacity. */
    static seq_t *seq = NULL;
    static size_t len = 0;
    static size_t cap = 0;

    /* Initialise the first two terms if this is the first call. */
    if (seq == NULL) {
        cap = 4;                     /* give some spare room */
        seq = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!seq) return (seq_t)0;   /* out‑of‑memory: return 0 (undefined) */
        seq[0] = (seq_t)1;
        seq[1] = (seq_t)16;
        len   = 2;
    }

    /* Grow the buffer if we need to store more values. */
    if ((idx_t)len <= n && (size_t)n + 1 > cap) {
        size_t new_cap = cap * 2;
        if (new_cap < (size_t)n + 1) new_cap = (size_t)n + 1;
        seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        if (!seq) return (seq_t)0;   /* out‑of‑memory: return 0 (undefined) */
        cap = new_cap;
    }

    /* Compute any missing terms up to index n. */
    for (idx_t i = (idx_t)len; i <= n; ++i) {
        size_t ui = (size_t)i;
        seq[ui] = (seq_t)8 * seq[ui - 1] - (seq_t)8 * seq[ui - 2];
    }
    if ((idx_t)len <= n) len = (size_t)n + 1;

    return seq[(size_t)n];
}
