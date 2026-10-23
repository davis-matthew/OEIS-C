/*
 * A026514  a(n) = least positive integer > a(n-1) and not (a(i)+a(j))^2 +
 *          a(k)^2 for 1<=i<=j<=k<=n.
 * https://oeis.org/A026514
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 30, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements OEIS sequence A026514.
   a(n) = least positive integer > a(n‑1) that is NOT representable as
   (a(i)+a(j))^2 + a(k)^2 for 1 ≤ i ≤ j ≤ k ≤ n.  The function builds
   the sequence incrementally and caches previously computed values. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

/* Incrementally build and return the nth term of the sequence. */
seq_t a(idx_t n) {
    /* One‑based cache: seq[0] is unused. */
    static seq_t *seq      = NULL;
    static idx_t seq_len   = 0;  /* highest index computed */
    static idx_t seq_cap   = 0;  /* maximum allocated index */

    /* Allocate if needed. */
    if (n > seq_cap) {
        idx_t new_cap = seq_cap ? seq_cap << 1 : 8;
        while (new_cap < n + 1) new_cap <<= 1;            /* ensure room for n */
        seq_t *new_seq = (seq_t*)malloc((new_cap + 1) * sizeof(seq_t));
        if (!new_seq) abort();                          /* allocation failed */

        /* Copy existing elements. */
        for (idx_t i = 1; i <= seq_len; ++i) new_seq[i] = seq[i];

        /* Free old storage. */
        free(seq);
        seq      = new_seq;
        seq_cap  = new_cap;
    }

    /* Compute missing terms up to n. */
    for (idx_t m = seq_len + 1; m <= n; ++m) {
        seq_t candidate = (m == 1) ? 1 : seq[m - 1] + 1;

        for (;;) {
            bool forbidden = false;

            /* Only consider k such that a(k)^2 ≤ candidate. */
            for (idx_t k = 1; k <= m - 1; ++k) {
                seq_t ak     = seq[k];
                seq_t ak_sq  = ak * ak;
                if (ak_sq > candidate) break;

                seq_t rem = candidate - ak_sq;
                seq_t t   = floor_sqrt(rem);
                if (t * t != rem) continue;   /* remainder not a perfect square */

                /* Check if t can be expressed as a sum of two a's
                 * among indices ≤ k.  Two‑pointer search works because
                 * the sequence is increasing. */
                idx_t i = 1, j = k;
                while (i <= j) {
                    seq_t sum = seq[i] + seq[j];
                    if (sum == t) { forbidden = true; break; }
                    if (sum < t) ++i;
                    else          --j;
                }
                if (forbidden) break;
            }

            if (!forbidden) break;          /* candidate accepted */
            ++candidate;                    /* try next integer */
        }

        seq[m] = candidate;
    }

    seq_len = n;   /* cache now contains n terms */
    return seq[n];
}
