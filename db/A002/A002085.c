/*
 * A002085  From the expansion of sinh(x) / cos(x): a(n) = odd part of
 *          A002084(n).
 * https://oeis.org/A002085
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = odd part of A002084(n), where A002084(n) = b(2n+1) and b
 *            = boustrophedon (Seidel-Entringer) transform of the sinh
 *            coefficients s(k) = k mod 2: T(k,0) = s(k), T(k,j) = T(k,j-1) +
 *            T(k-1,k-j), b(k) = T(k,k); e.g.f. (sec x + tan x)*sinh x, whose
 *            odd part is sinh(x)/cos(x)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    idx_t m = 2 * n + 1;
    idx_t k, j;
    int W = 0;
    seq_t x = 1;
    seq_t *pl, *ph, *cl, *ch, *t;
    seq_t lo, hi;

    /* bit width of seq_t (capped: arbitrary precision never carries into hi) */
    while (x != 0 && W < 200) { x <<= 1; W++; }

    pl = (seq_t *)calloc(m + 2, sizeof(seq_t));
    ph = (seq_t *)calloc(m + 2, sizeof(seq_t));
    cl = (seq_t *)calloc(m + 2, sizeof(seq_t));
    ch = (seq_t *)calloc(m + 2, sizeof(seq_t));
    for (k = 0; k <= m + 1; k++) { pl[k] = 0; ph[k] = 0; cl[k] = 0; ch[k] = 0; }

    /* row 0: T(0,0) = s(0) = 0 */
    pl[0] = 0; ph[0] = 0;
    for (k = 1; k <= m; k++) {
        cl[0] = (k & 1) ? 1 : 0;
        ch[0] = 0;
        for (j = 1; j <= k; j++) {
            seq_t sl = cl[j - 1] + pl[k - j];
            seq_t sh = ch[j - 1] + ph[k - j];
            if (sl < cl[j - 1]) sh += 1;   /* carry in fixed-width double-word arithmetic */
            cl[j] = sl;
            ch[j] = sh;
        }
        t = pl; pl = cl; cl = t;
        t = ph; ph = ch; ch = t;
    }

    lo = pl[m];
    hi = ph[m];
    free(pl); free(ph); free(cl); free(ch);

    /* remove all factors of 2 from the double-word value hi:lo */
    while (((lo & 1) == 0) && (lo != 0 || hi != 0)) {
        lo >>= 1;
        if (W < 200 && (hi & 1) != 0) lo |= ((seq_t)1 << (W - 1));
        hi >>= 1;
    }
    return lo;
}
