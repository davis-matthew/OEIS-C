/*
 * A143952  Triangle read by rows: T(n,k) is the number of Dyck paths of
 *          semilength n having k peak plateaux (0<=k<=floor(n/2)). A peak
 *          plateau is a run of consecutive peaks that is preceded by an
 *          upstep and followed by a down step; a peak consists of an upstep
 *          followed by a downstep.
 * https://oeis.org/A143952
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the nth entry of the triangle T(n,k) of Dyck paths with
 *            k peak plateaux. For the given linear index n we find the unique
 *            pair (row,col) such that offset_row(row) <= n <
 *            offset_row(row+1) and col = n - offset_row(row). Then a(n) =
 *            T(row,col) where T(R,K) = Σ_{s=1..R} Narayana(R-s,K) * C(2R...
 * Range:     u8 n <= 16, u16 n <= 43, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   binom, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"     /* binom(idx_t a, idx_t b) -> seq_t */
#include "../utils/seq_mul.h"   /* seq_mul(seq_t a, seq_t b) -> seq_t */

static seq_t T_row_k(idx_t R, idx_t K)
{
    /* Special cases */
    if (R == 0)            /* only K == 0 is legal */
        return (seq_t)1;
    if (K == 0)
        return (seq_t)1;

    seq_t total = 0;

    for (idx_t s = 1; s <= R; ++s) {
        idx_t m = R - s;               /* m = R - s */
        if (m < K)                    /* Narayana(m,K) is zero when m < K */
            break;

        /* Narayana(m, K) = C(m,K) * C(m,K-1) / m */
        seq_t nar = binom(m, K);
        nar = seq_mul(nar, binom(m, K - 1));
        nar = nar / (seq_t)m;

        idx_t b_n = 2 * R - s - K;
        idx_t b_k = s - K;
        seq_t bin_val = binom(b_n, b_k);

        total = total + seq_mul(nar, bin_val);
    }
    return total;
}

seq_t a(idx_t n)
{
    /* Find the row r and offset of that row.  Each row r has length
       L(r) = floor(r/2) + 1. */
    idx_t r = 0;
    idx_t offset = 0;                 /* starting index of current row */

    while (offset + (r / 2 + 1) <= n) {
        offset += (r / 2 + 1);
        ++r;
    }

    idx_t k = n - offset;             /* column within the row */
    return T_row_k(r, k);
}
