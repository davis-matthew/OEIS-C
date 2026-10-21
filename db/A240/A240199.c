/*
 * A240199  Area under the path specified by n-th composition.
 * https://oeis.org/A240199
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = area under the path specified by the n‑th composition in
 *            A066099, where the path has run lengths given by the composition
 *            followed by its reversal
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t if needed, not used here */

seq_t a(idx_t n)
{
    /* a(0) = 0 by definition */
    if (n == 0)
        return 0;

    /* Determine the row r in which the n‑th composition lies.
       Row r contains 2^(r‑1) compositions, and the first composition
       of row r starts at index 2^(r‑1). */
    idx_t r = 1;
    idx_t tmp = n;
    while (tmp > 1) {          /* find r such that 2^(r‑1) <= n < 2^r */
        tmp >>= 1;
        ++r;
    }

    /* Position of this composition within its row */
    idx_t base   = (idx_t)1 << (r - 1);   /* 2^(r‑1) */
    idx_t offset = n - base;              /* 0‑based within row */

    /* Decode the offset into the composition.
       Bits of offset, read from most significant to least
       (there are r‑1 bits), indicate whether there is a split after
       the current part (1) or not (0). */
    const idx_t len_bits = r - 1;
    idx_t parts[64];               /* r <= 64 for 64‑bit n */
    idx_t k = 0;                   /* number of parts */
    idx_t curr = 1;                /* length of current part */

    for (idx_t i = 0; i < len_bits; ++i) {
        idx_t bit = (offset >> (len_bits - 1 - i)) & 1ULL;
        if (bit) {                 /* split here */
            parts[k++] = curr;
            curr = 1;
        } else {                   /* extend current part */
            ++curr;
        }
    }
    parts[k++] = curr;              /* final part */

    /* Compute the area under the path.
       The runs are parts[0..k‑1] followed by parts[k‑1]..parts[0].
       Even‑indexed runs are horizontal (add to width),
       odd‑indexed runs are vertical (add run * current width). */
    seq_t area   = 0;
    seq_t horiz  = 0;

    for (idx_t i = 0; i < 2 * k; ++i) {
        idx_t run = (i < k) ? parts[i] : parts[2 * k - 1 - i];
        if ((i & 1ULL) == 0) {          /* horizontal step */
            horiz += (seq_t)run;
        } else {                        /* vertical step */
            area += (seq_t)run * horiz;
        }
    }

    return area;
}
