/*
 * A137695  Tower of Hanoi with p pegs: A(p,n) = number of moves needed for n
 *          disks, using Frame's or Stewart's algorithm, read by columns of
 *          the upper right triangle of rows p >= 3, columns n >= p-2.
 * https://oeis.org/A137695
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nice, nonn, tabl, changed
 * Formula:   A137695(n) – number of moves in the Frame–Stewart algorithm for
 *            a triangular matrix of (p,n) with p ≥ 3, n ≥ p‑2, read by
 *            columns. The implementation follows the closed form: A(p,n) =
 *            Σ_{t=0}^{s-1} 2^t·C(p‑3+t, p‑3) + 2^s·(n – C(p‑3+s, p‑2)), where
 *            s = max{ k ∈ ℕ | n > C(p‑3+k, p‑2) }. The functi...
 * Range:     u8 n <= 36, u16 n <= 136, u32 n <= 528, u64 n <= 2080,
 *            u128 n <= 8256, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Find the column (disk count) by solving k(k+1)/2 ≥ n. */
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        if (mid * (mid + 1) / 2 >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t disk = low;                       /* column index = n (disk count) */
    idx_t prev = (disk - 1) * disk / 2;     /* entries before this column   */
    idx_t pos   = n - prev - 1;             /* 0‑based offset in column    */
    idx_t peg   = 3 + pos;                  /* row index (number of pegs) */

    /* Determine s = max{k | disk > C(peg‑3 + k, peg‑2)}. */
    idx_t s = 0;
    while ((seq_t)disk > binomial(peg - 3 + s, peg - 2))
        ++s;
    if (s > 0)  --s;                       /* s is now the maximum satisfying k */

    /* Compute the sum Σ_{t=0}^{s-1} 2^t · C(peg‑3+t, peg‑3) */
    seq_t part1 = 0;
    seq_t power = 1;                        /* 2^0 */
    for (idx_t t = 0; t < s; ++t) {
        seq_t b = binomial(peg - 3 + t, peg - 3);
        part1 += power * b;
        power <<= 1;                        /* next power of two */
    }

    /* 2^s · (disk – C(peg‑3+s, peg‑2)) */
    seq_t remain = (seq_t)disk - binomial(peg - 3 + s, peg - 2);
    seq_t part2  = power * remain;          /* power now equals 2^s */

    return part1 + part2;
}
