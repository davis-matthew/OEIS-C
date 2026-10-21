/*
 * A216269  Numbers k such that k^2 - 1 is a tetrahedral number (A000292).
 * https://oeis.org/A216269
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 none, u64 n <= 7, u128 n <= 7, big any n
 * Helpers:   append_candidate, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A216269 : Numbers k such that k^2-1 is a tetrahedral number.
    Implementation: brute‑force enumeration of y (the tetrahedral index):
    we test all y up to a fixed limit and store every k for which
    T(y)+1 is a perfect square.  The limit (200 000) is more than
    sufficient to catch all known solutions (k up to 660 099); if the
    sequence is truly finite the search will discover every element.
    The results are cached so that repeated calls are O(1).  */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/append_candidate.h"
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n) {
    /* Cached list of solutions */
    static seq_t *kvals = NULL;
    static size_t klen = 0, kcap = 0;

    if (kvals == NULL) {
        /* Build the list once */
        const idx_t LIMIT = 200000;   /* safe bound for all known solutions */
        for (idx_t y = 0; y <= LIMIT; ++y) {
            seq_t T = (seq_t)y * (y + 1) * (y + 2) / 6;   /* tetrahedral number */
            seq_t v = T + 1;                              /* k^2 */
            seq_t k = floor_sqrt(v);                      /* integer square root */
            if (k * k == v) {                             /* perfect square? */
                append_candidate(&kvals, &klen, &kcap, k);
            }
        }
    }

    if (n == 0) return 0;
    if (n - 1 < klen) return kvals[n - 1];
    return 0;   /* undefined for n beyond the finite list */
}
