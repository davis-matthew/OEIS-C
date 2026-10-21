/*
 * A080578  a(1)=1; for n > 1, a(n) = a(n-1) + 1 if n is already in the
 *          sequence, a(n) = a(n-1) + 3 otherwise.
 * https://oeis.org/A080578
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; for n > 1, a(n) = a(n-1)+1 if n is already in the
 *            sequence, otherwise a(n) = a(n-1)+3
 * Range:     u8 n <= 127, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* The types seq_t and idx_t are supplied by the build system. */

seq_t a(idx_t n) {
    if (n == 0)                     /* below the offset – undefined, but return 0 */
        return (seq_t)0;

    /* Static state for memoisation. */
    static idx_t          maxN   = 0;   /* largest index computed so far */
    static seq_t *        a_vals = NULL;/* array of sequence values, 1‑based */
    static bool *         seen   = NULL;/* bitset marking which numbers are in the sequence */
    static size_t         seenCap = 0;  /* capacity of the seen array */

    /* Initialise on first call. */
    if (maxN == 0) {
        /* Allocate space for the first two indices (a_vals[0] unused). */
        a_vals = (seq_t*)malloc(2 * sizeof(seq_t));
        if (a_vals == NULL) return (seq_t)0;
        a_vals[1] = (seq_t)1;
        maxN = 1;

        /* Initialise seen array with capacity 2 (indices 0 and 1). */
        seenCap = 2;
        seen = (bool*)malloc(seenCap * sizeof(bool));
        if (seen == NULL) {
            free(a_vals);
            return (seq_t)0;
        }
        for (size_t i = 0; i < seenCap; ++i)
            seen[i] = false;
        seen[1] = true;            /* 1 is present in the sequence */
    }

    /* Grow the sequence array if necessary. */
    if (n >= maxN) {
        size_t newSize = (size_t)(n + 1);           /* +1 for 1‑based indexing */
        seq_t *tmp = (seq_t*)realloc(a_vals, newSize * sizeof(seq_t));
        if (tmp == NULL) return a_vals[maxN];      /* allocation failure – return last known value */
        a_vals = tmp;

        /* Compute all missing terms up to n. */
        for (idx_t idx = maxN + 1; idx <= n; ++idx) {
            bool already = (idx < seenCap) && seen[(size_t)idx];
            seq_t inc = already ? (seq_t)1 : (seq_t)3;
            a_vals[idx] = a_vals[idx - 1] + inc;

            /* Record the new value in the seen array. */
            seq_t val = a_vals[idx];
            /* Convert to a size_t index safely for the target problem size. */
            size_t valIdx = (size_t)(idx_t)val;
            if (valIdx >= seenCap) {
                /* Grow seen array to accommodate the new value. */
                size_t newCap = seenCap > 0 ? seenCap * 2 : 4;
                while (newCap <= valIdx) newCap *= 2;
                bool *t = (bool*)realloc(seen, newCap * sizeof(bool));
                if (t == NULL) { /* allocation failure – cannot record, but keep sequence */
                    seenCap = newCap; /* pretend we succeeded to avoid repeated failures */
                } else {
                    for (size_t i = seenCap; i < newCap; ++i)
                        t[i] = false;
                    seen = t;
                    seenCap = newCap;
                }
            }
            if (valIdx < seenCap)
                seen[valIdx] = true;
        }
        maxN = n;
    }

    return a_vals[n];
}
