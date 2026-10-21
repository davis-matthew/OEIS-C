/*
 * A210530  T(n,k) = (k + 3*n - 2 - (k+n-2)*(-1)^(k+n))/2 n, k > 0, read by
 *          antidiagonals.
 * https://oeis.org/A210530
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is read along the antidiagonals of the table * T(n,k) = (k
 *            + 3*n - 2 - (k+n-2)*(-1)^(k+n)) / 2 . * For a given index n
 *            (1‑based), find the diagonal d with * d(d+1)/2 ≥ n, let p = n –
 *            (d-1)d/2 – 1. Then * row = p+1, col = d-p. The table entry
 *            simplifies to * if (row+col even) => row * else =>...
 * Range:     u8 n <= 8511, u16 n <= 9870, u32 n <= 9870, u64 n <= 9870,
 *            u128 n <= 9870, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n) {
    /* Find diagonal d  (d>=1) such that d*(d+1)/2 >= n  */
    idx_t d = 1;
    while ((unsigned __int128)d * (d + 1) / 2 < n)
        ++d;

    /* Previous total entries before diagonal d */
    unsigned __int128 prev = (unsigned __int128)(d - 1) * d / 2;
    idx_t offset = (idx_t)(n - prev - 1);                 /* 0‑based within diagonal */

    /* coordinates in the table */
    seq_t sn = (seq_t)(offset + 1);               /* row n */
    seq_t sk = (seq_t)(d - offset);               /* column k */

    /* compute value using the simplified formula */
    if (((sn + sk) & (seq_t)1) == 0)  /* even sum → value = row */
        return sn;
    else                              /* odd sum  → value = column + 2*row - 2 */
        return sk + 2 * sn - 2;
}
