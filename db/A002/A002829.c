/*
 * A002829  Number of trivalent (or cubic) labeled graphs with 2n nodes.
 * https://oeis.org/A002829
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   with b(N) = a(N)/(2N)!, the e.g.f. ODE (Jovovic) gives 48(N+1)
 *            b(N+1) = (72N(N-1)+192N) b(N) + (4-72(N-1)(N-2)) b(N-1) -
 *            (36(N-2)(N-3)+36(N-2)+8) b(N-2) - 36(N-3) b(N-3) - (6(N-4)-4)
 *            b(N-4) + b(N-5), i.e. a(N+1) = (2N+1)/24 * Sum_{t=0..5} c_t *
 *            (2N)!/(2N-2t)! * a(N-t), a(0) = 1, a(m) = 0 for m < 0
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 8, u128 n <= 13,
 *            big any n
 * Helpers:   a2829_from128, a2829_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a2829_from128.h"
#include "../utils/a2829_small.h"

static seq_t a2829_win[6];
static idx_t a2829_top = 0;
static int a2829_init = 0;

seq_t a(idx_t n)
{
    __int128 s[14];
    int k;
    a2829_small(s);
    if (n <= 13) return a2829_from128((unsigned __int128)s[n]);

    if (!a2829_init || n < a2829_top) {
        for (k = 0; k < 6; k++)
            a2829_win[k] = a2829_from128((unsigned __int128)s[8 + k]);
        a2829_top = 13;
        a2829_init = 1;
    }

    while (a2829_top < n) {
        idx_t N = a2829_top;
        idx_t c0 = 72 * N * (N - 1) + 192 * N;
        idx_t c1 = 72 * (N - 1) * (N - 2) - 4;
        idx_t c2 = 36 * (N - 2) * (N - 3) + 36 * (N - 2) + 8;
        idx_t c3 = 36 * (N - 3);
        idx_t c4 = 6 * (N - 4) - 4;
        idx_t p1 = (2 * N) * (2 * N - 1);
        idx_t p2 = (2 * N - 2) * (2 * N - 3);
        idx_t p3 = (2 * N - 4) * (2 * N - 5);
        idx_t p4 = (2 * N - 6) * (2 * N - 7);
        idx_t p5 = (2 * N - 8) * (2 * N - 9);
        seq_t W, X, T;

        /* win[0] = a(N-5), ..., win[5] = a(N) */
        W = a2829_win[1] * c4;
        X = a2829_win[0] * p5;
        W -= X;
        W *= p4;
        X = a2829_win[2] * c3;
        W += X;
        W *= p3;
        X = a2829_win[3] * c2;
        W += X;
        W *= p2;
        X = a2829_win[4] * c1;
        W += X;
        W *= p1;
        T = a2829_win[5] * c0;
        T -= W;
        T *= (2 * N + 1);
        T /= 24;

        for (k = 0; k < 5; k++) a2829_win[k] = a2829_win[k + 1];
        a2829_win[5] = T;
        a2829_top++;
    }
    return a2829_win[5];
}
