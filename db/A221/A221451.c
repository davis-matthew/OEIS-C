/*
 * A221451  Number of 6 X n binary arrays with every 0 a horizontal or
 *          antidiagonal neighbor to some 1.
 * https://oeis.org/A221451
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 6 × n binary arrays in which every 0 has a
 *            horizontal or antidiagonal neighbour that is a 1. (Indices start
 *            at 1.)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  This implementation uses a DP over pairs of consecutive column masks.
    It runs in O(n·64³) time, which is fast enough for n up to 10⁴. */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    const int W   = 6;            /* number of rows        */
    const int M   = 1 << W;       /* number of column masks = 64 */
    const int SZ  = M * M;        /* number of state pairs */

    /* --------------------------------------------------------------------
       Precomputation of auxiliary data.
       -------------------------------------------------------------------- */
    static uint8_t  unsat[64][64];     /* zeros in second mask that are not
                                        satisfied by the first mask        */
    static uint64_t reqRbits[64];      /* for each unsatisfied mask,
                                        bit r is set if column r would
                                        satisfy the unsatisfied zeros      */
    static int      init_done = 0;

    if (!init_done) {
        /* unsat matrix */
        for (int p = 0; p < M; ++p)
            for (int q = 0; q < M; ++q) {
                uint8_t m = 0;
                for (int r = 0; r < W; ++r) {
                    if ((q & (1 << r)) == 0) {                    /* zero */
                        int has_left   = !!(p & (1 << r));
                        int has_upLeft = (r > 0) && (p & (1 << (r - 1)));
                        if (!has_left && !has_upLeft)
                            m |= (1 << r);
                    }
                }
                unsat[p][q] = m;
            }

        /* precompute for every unsatisfied mask the set of next‑column masks
           that can satisfy it (with a right neighbour or a down‑right neighbor) */
        for (int s = 0; s < M; ++s) {
            uint64_t bits = 0;
            for (int r = 0; r < M; ++r) {
                /* check: (r | r>>1) has a 1 at each position required by s   */
                if ((s & ~( (uint8_t) r | (uint8_t)(r >> 1) )) == 0)
                    bits |= (1ULL << r);
            }
            reqRbits[s] = bits;
        }
        init_done = 1;
    }

    /* --------------------------------------------------------------------
       DP arrays: dpPrev[prevPrev][prev]  and  dpCurr[prev][cur]
       -------------------------------------------------------------------- */
    static seq_t dpPrev[64][64];
    static seq_t dpCurr[64][64];

    /* initialise dpPrev for column 1.  The imaginary previous column is all
       zeros, so dpPrev[0][mask] = 1 for every mask.                         */
    for (int i = 0; i < M; ++i)
        for (int j = 0; j < M; ++j)
            dpPrev[i][j] = (seq_t)0;
    for (int m = 0; m < M; ++m)
        dpPrev[0][m] = (seq_t)1;

    /* run n-1 transitions (moving through columns 2 .. n) */
    for (idx_t step = 1; step < n; ++step) {
        /* zero dpCurr */
        for (int i = 0; i < M; ++i)
            for (int j = 0; j < M; ++j)
                dpCurr[i][j] = (seq_t)0;

        for (int prevPrev = 0; prevPrev < M; ++prevPrev) {
            for (int prev = 0; prev < M; ++prev) {
                seq_t val = dpPrev[prevPrev][prev];
                if (!val) continue;

                uint8_t need = unsat[prevPrev][prev];
                uint64_t bits = reqRbits[need];
                while (bits) {
                    int cur = __builtin_ctzll(bits);   /* index of LSB */
                    bits &= bits - 1;                  /* clear that bit */
                    dpCurr[prev][cur] += val;
                }
            }
        }

        /* copy dpCurr to dpPrev for next iteration */
        for (int i = 0; i < M; ++i)
            for (int j = 0; j < M; ++j)
                dpPrev[i][j] = dpCurr[i][j];
    }

    /* final answer: sum over states where the last column is fully satisfied
       (i.e., unsatisfied mask of the last column equals 0) */
    seq_t ans = (seq_t)0;
    for (int prev = 0; prev < M; ++prev)
        for (int cur = 0; cur < M; ++cur)
            if (unsat[prev][cur] == 0)
                ans += dpPrev[prev][cur];

    return ans;
}
