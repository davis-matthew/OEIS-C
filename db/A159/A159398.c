/*
 * A159398  Number of n X n arrays of squares of integers with every
 *          (n-1)X(n-1) subblock summing to 16.
 * https://oeis.org/A159398
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×n arrays of integer squares such that every
 *            (n-1)×(n-1) subblock sums to 16. The computation is performed
 *            via a counting argument that reduces the constraints to a small
 *            set of linear equations on the sums of the border cells and the
 *            interior cells
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 9, u128 n <= 22,
 *            big any n
 * Helpers:   binom_large, ways
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The algorithm enumerates all possible corner values (0,1,4,9,16) that
   satisfy the relation a0 + a3 = a1 + a2.  For each such quadruple it
   iterates over all feasible sums of the four border lines (F, L, Lc, Rc)
   and uses combinatorial counting to determine the number of ways to fill
   the corresponding cells with squares summing to those values.  The
   remaining interior cells must sum to a value T determined by the
   equations, and the number of ways to achieve this is likewise
   counted.  All arithmetic uses the generic integer type `seq_t`
   and the index type `idx_t`.                                                */
#include <stdint.h>
#include "../utils/binom_large.h"
#include "../utils/ways.h"

seq_t a(idx_t n)
{
    if (n < (idx_t)3) return (seq_t)0;          /* offset not required */

    idx_t m = n - (idx_t)2;                       /* border cell count per side */
    idx_t t = m * m;                              /* interior cell count */

    seq_t w_m[17];
    seq_t w_t[17];

    for (idx_t s = (idx_t)0; s <= (idx_t)16; ++s) {
        w_m[s] = ways(m, s);
        w_t[s] = ways(t, s);
    }

    static const uint8_t squares[5] = {0, 1, 4, 9, 16};

    seq_t total = (seq_t)0;

    for (int a0_idx = 0; a0_idx < 5; ++a0_idx) {
        int a0 = squares[a0_idx];
        for (int a1_idx = 0; a1_idx < 5; ++a1_idx) {
            int a1 = squares[a1_idx];
            for (int a2_idx = 0; a2_idx < 5; ++a2_idx) {
                int a2 = squares[a2_idx];
                for (int a3_idx = 0; a3_idx < 5; ++a3_idx) {
                    int a3 = squares[a3_idx];
                    if (a0 + a3 != a1 + a2) continue;

                    int d1 = a1 - a3;
                    int d2 = a2 - a3;

                    for (int F = 0; F <= 16; ++F) {
                        int L = F + d1;
                        if (L < 0 || L > 16) continue;
                        seq_t wF = w_m[F];
                        if (wF == (seq_t)0) continue;
                        seq_t wL = w_m[L];
                        if (wL == (seq_t)0) continue;

                        for (int LC = 0; LC <= 16; ++LC) {
                            int Rc = LC + d2;
                            if (Rc < 0 || Rc > 16) continue;
                            seq_t wLC = w_m[LC];
                            if (wLC == (seq_t)0) continue;
                            seq_t wRC = w_m[Rc];
                            if (wRC == (seq_t)0) continue;

                            int T = 16 - (F + LC + a0);
                            if (T < 0 || T > 16) continue;
                            seq_t wInt = w_t[T];
                            if (wInt == (seq_t)0) continue;

                            seq_t contrib = wF * wL;
                            contrib = contrib * wLC;
                            contrib = contrib * wRC;
                            contrib = contrib * wInt;
                            total = total + contrib;
                        }
                    }
                }
            }
        }
    }

    return total;
}
