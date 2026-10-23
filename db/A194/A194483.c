/*
 * A194483  Number of ways to arrange 6 indistinguishable points on an n X n X
 *          n triangular grid so that no four points are in the same row or
 *          diagonal.
 * https://oeis.org/A194483
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (7 n^12 + 42 n^11 - 105 n^10 - 1918 n^9 + 6097 n^8 + 6214
 *            n^7 - 39011 n^6 + 16198 n^5 + 73556 n^4 - 62776 n^3 - 40544 n^2
 *            + 42240 n) / 322560
 * Range:     u8 none, u16 n <= 3, u32 n <= 5, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1U) return 0;                     /* offset starts at 1 */

    seq_t sn = (seq_t)n;
    seq_t pown[13];                           /* pown[i] = n^i   */
    pown[0] = (seq_t)1;
    for (idx_t i = 1; i <= 12; ++i)
        pown[i] = pown[i - 1] * sn;

    /* Positive terms */
    seq_t pos = 0;
    pos += (seq_t)7    * pown[12];
    pos += (seq_t)42   * pown[11];
    pos += (seq_t)6097 * pown[8];
    pos += (seq_t)6214 * pown[7];
    pos += (seq_t)16198* pown[5];
    pos += (seq_t)73556* pown[4];
    pos += (seq_t)42240* pown[1];

    /* Negative terms (subtracted later) */
    seq_t neg = 0;
    neg += (seq_t)105  * pown[10];
    neg += (seq_t)1918 * pown[9];
    neg += (seq_t)39011* pown[6];
    neg += (seq_t)62776* pown[3];
    neg += (seq_t)40544* pown[2];

    seq_t num = pos - neg;                    /* numerator of the fraction */
    const seq_t denom = 322560;

    return num / denom;
}
