/*
 * A023709  Numbers with no 1's in their base 4 expansion.
 * https://oeis.org/A023709
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = value obtained by converting n‑1 from base 3 to base 4
 *            using the digit mapping 0→0, 1→2, 2→3. These are exactly the
 *            numbers that contain no digit 1 in their base‑4 expansion
 * Range:     u8 n <= 81, u16 n <= 6561, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static const seq_t map_digit[3] = { 0, 2, 3 };

seq_t a(idx_t n)
{
    /* Sequence offset is 1; a(1) = 0.  Guard against n==0 as a
       defensive programming measure. */
    if (n == 0)
        return 0;

    idx_t k = n - 1;               /* base‑3 number that will be mapped */
    unsigned char digs[64];        /* maximum 64 digits for n up to 2^64‑1 */
    size_t cnt = 0;

    /* Extract base‑3 digits (least‑significant first). */
    while (k > 0) {
        digs[cnt++] = (unsigned char)(k % 3);
        k /= 3;
    }

    /* If n == 1, k == 0 and cnt == 0 → the only term is 0. */
    if (cnt == 0)
        return 0;

    seq_t res = 0;

    /* Construct the base‑4 number from most‑significant to least. */
    for (size_t i = cnt; i-- > 0; ) {
        res = res * 4 + map_digit[digs[i]];
    }

    return res;
}
