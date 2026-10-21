/*
 * A239132  Pattern of word endings in the lines of a Sestina.
 * https://oeis.org/A239132
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, full, fini
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A239132: Pattern of word endings in a Sestina.
 * For stanza i (i = 0 for the first stanza) let
 *   p = (1, 6, 3, 5, 4, 2)
 * be the cyclic permutation of the six end‑word labels.
 * The labels for stanza i+1 are obtained from those for stanza i
 * by applying p once (i.e. shifting each label according to p).
 * The first stanza has labels 1,2,3,4,5,6.
 * a(n) is the label of the line number n (1‑based).
 */

#include <stddef.h>  /* for size_t (used only in static array size) */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                 /* OEIS offset starts at 1 */

    /* Zero‑based index of the requested line */
    idx_t idx = n - 1;

    /* Which stanza (0‑based) and which position within that stanza */
    idx_t stanza = idx / 6;
    idx_t pos    = idx % 6;

    /* The permutation p in zero‑based indices: 0→5, 5→2, 2→4, 4→3, 3→1, 1→0 */
    static const idx_t perm[6] = {5, 0, 4, 1, 3, 2};

    /* Only 6 applications can change the position because p has order 6 */
    idx_t e = stanza % 6;
    idx_t final_pos = pos;
    while (e--)
        final_pos = perm[final_pos];

    /* Convert zero‑based position back to the label (1‑based) */
    return (seq_t)(final_pos + 1);
}
