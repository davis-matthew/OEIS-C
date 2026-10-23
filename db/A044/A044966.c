/*
 * A044966  Numbers whose base-3 representation contains no 0's and exactly
 *          one 1.
 * https://oeis.org/A044966
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044966 – numbers whose base‑3 representation contains no 0's
   and exactly one 1.
   For an index n≥1 let L be the smallest integer with L(L+1)/2 ≥ n
   (so the first L terms are a(1)…a(L), the next L terms are a(L+1)…a(2L), …).
   Within the block of length L the terms are sorted in ascending order.
   With offset = n – L(L−1)/2 − 1 (0‑based) and k = L−1−offset,
   the n‑th term is

        a(n) = 3^L − 1 − 3^k .
   (All arithmetic uses the arbitrary‑precision type seq_t.) */

#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    /* Handle out‑of‑range indices gracefully (not used by tests). */
    if (n == 0) return 0;

    /* Find minimal L such that L(L+1)/2 ≥ n. */
    idx_t L = 1;
    while ((seq_t)L * (L + 1) / 2 < (seq_t)n)
        ++L;

    /* triangular number of previous block */
    seq_t triPrev = (seq_t)(L - 1) * (seq_t)L / 2;

    /* offset within current block (0‑based) */
    idx_t offset = (idx_t)((seq_t)n - triPrev - 1);

    /* position of the single 1 counted from the least significant digit */
    idx_t k = (L - 1) - offset;

    /* compute the term: 3^L − 1 − 3^k */
    seq_t val = pow3(L) - 1 - pow3(k);
    return val;
}
