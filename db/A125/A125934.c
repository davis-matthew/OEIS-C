/*
 * A125934  Sprague-Grundy values for octal game .7337.
 * https://oeis.org/A125934
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sprague–Grundy value for octal game .7337
 * Range:     u8 n <= 92, u16 n <= 92, u32 n <= 92, u64 n <= 92, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* Octal digits .7337: index 1..4 (0 otherwise) */
static const int _dig_len = 4;
static const unsigned char _dig[5] = { 0, 7, 3, 3, 7 };

/* Cache of computed Grundy values (sg[0] = 0). */
static seq_t *sg = NULL;           /* array of SG values */
static idx_t max_n = 0;           /* highest index computed so far */

/* Temporary array used for mex computation. */
static unsigned char *seen = NULL; /* flags for values up to max_n+2 */
static size_t seen_cap = 0;         /* capacity of the 'seen' array */

/* Compute SG value for heap of size n. */
seq_t a(idx_t n)
{
    if (n == 0)                        /* not used for OEIS offset, harmless */
        return 0;

    /* Ensure cache has space for indices up to n. */
    if (n > max_n) {
        /* Allocate SG array if first time. */
        if (sg == NULL) {
            sg = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
            if (sg == NULL) abort();
            sg[0] = 0;               /* SG(0) = 0 */
        } else if (n + 1 > max_n + 1) {
            seq_t *tmp = (seq_t *)realloc(sg, (size_t)(n + 1) * sizeof(seq_t));
            if (tmp == NULL) abort();
            sg = tmp;
        }

        /* Allocate 'seen' array for mex up to n+1. */
        size_t needed = (size_t)n + 3;
        if (seen_cap < needed) {
            unsigned char *tmp = (unsigned char *)realloc(seen, needed);
            if (tmp == NULL) abort();
            seen = tmp;
            seen_cap = needed;
        }

        /* Compute SG values from previous max_n+1 up to n. */
        for (idx_t size = max_n + 1; size <= n; ++size) {
            /* Reset seen array for this size (indices 0..size+1). */
            memset(seen, 0, (size_t)(size + 2));

            /* Consider all possible moves where k stones are removed. */
            for (idx_t k = 1; k <= size; ++k) {
                unsigned char d = (k <= _dig_len ? _dig[(int)k] : 0);
                if (d == 0) continue;         /* no moves for this k */

                idx_t rem = size - k;
                for (idx_t L = 0; L <= rem; ++L) {
                    idx_t R = rem - L;

                    /* Check whether this split is allowed by the digit. */
                    if (L == 0 && R == 0) {
                        /* terminating move: requires bits 0 and 1. */
                        if (!(d & 1) || !(d & 2)) continue;
                    } else if (L == 0) {
                        if (!(d & 1)) continue;   /* must be allowed to leave only right */
                    } else if (R == 0) {
                        if (!(d & 2)) continue;   /* must be allowed to leave only left */
                    } else {
                        if (!(d & 4)) continue;   /* must be allowed to leave two non‑empty heaps */
                    }

                    seq_t g = sg[(size_t)L] ^ sg[(size_t)R];
                    if (g <= (seq_t)(size + 1))
                        seen[(size_t)g] = 1;
                }
            }

            /* mex: smallest non‑negative integer not seen. */
            seq_t mex = 0;
            while (mex <= size + 1 && seen[(size_t)mex]) ++mex;
            sg[(size_t)size] = mex;
        }

        max_n = n;
    }

    return sg[(size_t)n];
}
