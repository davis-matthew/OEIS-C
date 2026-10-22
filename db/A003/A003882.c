/*
 * A003882  Degrees of irreducible representations of group L2(11).
 * https://oeis.org/A003882
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   degrees of irreducible characters of PSL(2,q), q = 11 (q == 3
 *            mod 4), in increasing order: 1, (q-1)/2 [x2], q-1 [x(q-3)/4], q,
 *            q+1 [x(q-3)/4]. Finite sequence: a(1..8)
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    const unsigned q = 11;
    unsigned degs[16];
    int cnt = 0, i, j;
    unsigned m = (q - 3) / 4;

    degs[cnt++] = 1;
    degs[cnt++] = (q - 1) / 2;
    degs[cnt++] = (q - 1) / 2;
    for (i = 0; i < (int)m; i++) degs[cnt++] = q - 1;
    degs[cnt++] = q;
    for (i = 0; i < (int)m; i++) degs[cnt++] = q + 1;

    /* sort (already ordered, but keep it robust) */
    for (i = 1; i < cnt; i++) {
        unsigned t = degs[i];
        j = i - 1;
        while (j >= 0 && degs[j] > t) { degs[j + 1] = degs[j]; j--; }
        degs[j + 1] = t;
    }

    if (n < 1 || n > (idx_t)cnt) return (seq_t)0;
    return (seq_t)degs[n - 1];
}
