/*
 * A184996  For each ordered partition of n with k numbers, remove 1 from each
 *          part and add the number k to get a new partition, until a
 *          partition is repeated. Among all ordered partitions of n, a(n)
 *          gives the maximum number of steps needed to reach a period.
 * https://oeis.org/A184996
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/*  Formula (OEIS A184996):
    a(n) = max number of steps in the partition transformation.
    Piecewise definition (valid for n ≥ 9):
        n = k(k+1)/2
              => a(n) = k^2 - 1
        n = (k^2 + k - 2)/2 - j ,  0 ≤ j ≤ (k-4)/2 , k ≥ 4
              => a(n) = k^2 - 3 - (k+1)·j
        n = (k^2 + k + 2)/2 + j ,  0 ≤ j ≤ (k-5)/2 , k ≥ 5
              => a(n) = k^2 - 1 - k·j
        n = (k^2 + 2k - 2 + (k mod 2))/2 + j ,
              0 ≤ j ≤ (2-(k mod 2)), k ≥ 4
              => a(n) = (k^2 + 4k - 2 + (k mod 2))/2 + j
    Small values (n = 1 .. 8) are handled explicitly:
        a(1)=0, a(2)=1, a(3)=3, a(4)=5, a(5)=7, a(6)=8,
        a(7)=9, a(8)=11.  */

/* Function required by the test harness. */
seq_t a(idx_t n)
{
    /* Small fixed values. */
    if (n <= 8) {
        switch (n) {
        case 1: return (seq_t)0;
        case 2: return (seq_t)1;
        case 3: return (seq_t)3;
        case 4: return (seq_t)5;
        case 5: return (seq_t)7;
        case 6: return (seq_t)8;
        case 7: return (seq_t)9;
        case 8: return (seq_t)11;
        default: /* never reaches */ return (seq_t)0;
        }
    }

    for (idx_t k = 1; ; ++k) {
        /* Triangular case */
        unsigned __int128 t128 = (unsigned __int128)k * (k + 1) / 2;
        idx_t t = (idx_t)t128;
        if (n == t) {
            return (seq_t)k * k - 1;
        }

        /* Formula 1 (k ≥ 4) */
        if (k >= 4) {
            unsigned __int128 base1_128 = ((unsigned __int128)k * k + k - 2) / 2;
            idx_t base1 = (idx_t)base1_128;
            idx_t jmax1 = (k - 4) / 2;
            for (idx_t j = 0; j <= jmax1; ++j) {
                idx_t cand = base1 - j;
                if (cand == n) {
                    seq_t kk = (seq_t)k;
                    seq_t jj = (seq_t)j;
                    return kk * kk - 3 - (kk + 1) * jj;
                }
            }
        }

        /* Formula 2 (k ≥ 5) */
        if (k >= 5) {
            unsigned __int128 base2_128 = ((unsigned __int128)k * k + k + 2) / 2;
            idx_t base2 = (idx_t)base2_128;
            idx_t jmax2 = (k - 5) / 2;
            for (idx_t j = 0; j <= jmax2; ++j) {
                idx_t cand = base2 + j;
                if (cand == n) {
                    seq_t kk = (seq_t)k;
                    seq_t jj = (seq_t)j;
                    return kk * kk - 1 - kk * jj;
                }
            }
        }

        /* Formula 3 (k ≥ 4) */
        if (k >= 4) {
            unsigned __int128 base3_128 = ((unsigned __int128)k * k + 2 * k - 2 + (k & 1)) / 2;
            idx_t base3 = (idx_t)base3_128;
            idx_t jmax3 = (k & 1) ? 1 : 2;
            for (idx_t j = 0; j <= jmax3; ++j) {
                idx_t cand = base3 + j;
                if (cand == n) {
                    unsigned __int128 a128 = ((unsigned __int128)k * k + 4 * k - 2 + (k & 1)) / 2 + j;
                    return (seq_t)a128;
                }
            }
        }

        /* Safe guard to avoid infinite loop (won't happen for valid n) */
        if (k > n + 5)
            break;
    }

    /* Should never reach here for n ≥ 1. */
    return (seq_t)0;
}
