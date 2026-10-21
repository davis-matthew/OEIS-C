/*
 * A220613  Number of ways to reciprocally link elements of an nX7 array
 *          either to themselves or to exactly two horizontal and vertical
 *          neighbors, without consecutive collinear links.
 * https://oeis.org/A220613
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A220613
   Empirical recurrence (order 14):
   a(1) = 1,  a(2) = 21,  a(3) = 85,  a(4) = 781,
   a(5) = 4673,  a(6) = 34981,  a(7) = 233265,
   a(8) = 1653229,  a(9) = 11354857,  a(10) = 79290645,
   a(11) = 548972737,  a(12) = 3817993565,  a(13) = 26491584361,
   a(14) = 184040357765.
   For n ≥ 15:
   a(n) = 5·a(n‑1) + 40·a(n‑2) − 144·a(n‑3) − 532·a(n‑4)
          + 1484·a(n‑5) + 3044·a(n‑6) − 6644·a(n‑7) − 8872·a(n‑8)
          + 13920·a(n‑9) + 13824·a(n‑10) − 13056·a(n‑11) − 10688·a(n‑12)
          + 4288·a(n‑13) + 3072·a(n‑14)
*/

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Initial 14 terms (offset 1) */
    static const seq_t init[14] = {
        (seq_t)  1ULL,
        (seq_t) 21ULL,
        (seq_t) 85ULL,
        (seq_t) 781ULL,
        (seq_t) 4673ULL,
        (seq_t) 34981ULL,
        (seq_t)233265ULL,
        (seq_t)1653229ULL,
        (seq_t)11354857ULL,
        (seq_t)79290645ULL,
        (seq_t)548972737ULL,
        (seq_t)3817993565ULL,
        (seq_t)26491584361ULL,
        (seq_t)184040357765ULL
    };

    if (n < 1) return (seq_t)0;         /* offset is 1 */
    if (n <= 14) return init[(size_t)n - 1];

    /* Buffer holding the last 14 terms:
       last[0] = a(k‑13), last[13] = a(k‑1) for current k   */
    seq_t last[14];
    for (size_t i = 0; i < 14; ++i) last[i] = init[i];

    for (idx_t cur = 15; cur <= n; ++cur)
    {
        seq_t next =
            5   * last[13]  + 40  * last[12]  - 144 * last[11] - 532 * last[10]
          + 1484 * last[9]  + 3044 * last[8]  - 6644 * last[7] - 8872 * last[6]
          + 13920* last[5]  + 13824* last[4]  - 13056*last[3] - 10688*last[2]
          + 4288 * last[1]  + 3072 * last[0];

        if (cur == n) return next;   /* result for requested n */

        /* Shift the buffer for the next iteration:
           last[0..12] = last[1..13]; last[13] = next   */
        for (size_t i = 0; i < 13; ++i) last[i] = last[i + 1];
        last[13] = next;
    }

    /* Unreachable path (compiler safety) */
    return (seq_t)0;
}
