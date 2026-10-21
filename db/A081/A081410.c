/*
 * A081410  a(n) = a(n-1) + a(n-2) + n (mod 3), with a(1)=a(2)=1.
 * https://oeis.org/A081410
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-2) + (n mod 3)
 * Range:     u8 n <= 12, u16 n <= 23, u32 n <= 46, u64 n <= 92, u128 n <= 184,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *buf = NULL;      /* buffer of computed terms, 0‑based for n=1 */
    static size_t len = 0;         /* number of terms stored */
    static size_t cap = 0;         /* allocated capacity */

    if (n < 1) return 0;           /* behaviour below offset is undefined */

    /* Ensure the first two terms are initialized */
    if (len == 0) {
        cap = 4;
        buf = (seq_t*)malloc(cap * sizeof(seq_t));
        buf[0] = (seq_t)1;          /* a(1) */
        buf[1] = (seq_t)1;          /* a(2) */
        len = 2;
    }

    /* If the requested index is already computed, return it */
    if ((size_t)n <= len) {
        return buf[n-1];
    }

    /* Grow the buffer as needed */
    if (len + (n - len) > cap) {
        size_t new_cap = cap;
        while (len + (n - len) > new_cap)
            new_cap *= 2;
        buf = (seq_t*)realloc(buf, new_cap * sizeof(seq_t));
        cap = new_cap;
    }

    /* Compute missing terms up to n */
    for (size_t i = len + 1; i <= n; ++i) {        /* i is 1‑based index */
        idx_t rem = i % 3;                         /* (i mod 3) */
        seq_t next = buf[i-2] + buf[i-3] + (seq_t)rem;
        buf[i-1] = next;
    }
    len = n;

    return buf[n-1];
}
