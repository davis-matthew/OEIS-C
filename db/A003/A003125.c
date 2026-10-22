/*
 * A003125  Value of an urn with n balls of type -1 and n+2 balls of type +1.
 * https://oeis.org/A003125
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   A(m,p) = A(m-1,p) + A(m,p-1), A(0,0)=0, A(1,0)=-1, A(0,1)=1, A=0
 *            for m<0 or p<0; B(m,p) = max{0, A(m,p) + B(m-1,p) + B(m,p-1)},
 *            B=0 for m<0 or p<0; a(n) = B(n-1, n+1) (OEIS offset 1, so a(1) =
 *            B(0,2) = 2). A is kept as sign+magnitude so it fits in unsigned
 *            seq_t
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   a003125_sadd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/a003125_sadd.h"

seq_t a(idx_t n)
{
    idx_t m, p, i, j, w;
    seq_t *Apm, *Acm, *Bp, *Bc, *ts, result;
    int *Aps, *Acs, *ti;

    if (n == 0) return 0;
    m = n - 1;
    p = n + 1;
    w = p + 1;

    Apm = (seq_t *)malloc(w * sizeof(seq_t));
    Acm = (seq_t *)malloc(w * sizeof(seq_t));
    Bp = (seq_t *)malloc(w * sizeof(seq_t));
    Bc = (seq_t *)malloc(w * sizeof(seq_t));
    Aps = (int *)malloc(w * sizeof(int));
    Acs = (int *)malloc(w * sizeof(int));

    /* row 0 */
    Apm[0] = 0; Aps[0] = 0; Bp[0] = 0;
    for (j = 1; j <= p; j++) {
        Apm[j] = 1; Aps[j] = 1;
        Bp[j] = (seq_t)j;
    }
    for (j = 0; j <= p; j++) { Acm[j] = 0; Acs[j] = 0; Bc[j] = 0; }

    for (i = 1; i <= m; i++) {
        seq_t s;
        Acm[0] = 1; Acs[0] = -1;
        for (j = 1; j <= p; j++)
            a003125_sadd(Apm[j], Aps[j], Acm[j - 1], Acs[j - 1], &Acm[j], &Acs[j]);

        for (j = 0; j <= p; j++) {
            s = Bp[j];
            if (j > 0) s += Bc[j - 1];
            if (Acs[j] >= 0) s += Acm[j];
            else if (s > Acm[j]) s -= Acm[j];
            else s = 0;
            Bc[j] = s;
        }

        ts = Apm; Apm = Acm; Acm = ts;
        ti = Aps; Aps = Acs; Acs = ti;
        ts = Bp; Bp = Bc; Bc = ts;
    }

    result = Bp[p];

    free(Apm); free(Acm); free(Bp); free(Bc);
    free(Aps); free(Acs);
    return result;
}
