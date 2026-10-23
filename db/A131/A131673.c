/*
 * A131673  Size of the largest BDD of symmetric Boolean functions of n
 *          variables when the sink nodes are counted.
 * https://oeis.org/A131673
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2 + Σ_{k=1..n} min(k, 2^(n+2-k) - 2)
 * Range:     u8 n <= 24, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   floor_log2_uint64, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent closed form (used below):
   a(0) = 1
   for n > 0:
     let b_n = ⌊log₂(n+4 - ⌊log₂(n+4)⌋)⌋
     a(n) = C(n+2-b_n, 2) + 2(2^b_n - b_n) */

#include "../utils/floor_log2_uint64.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;   /* a(0) */

    /* Compute b_n */
    idx_t tmp = n + 4;                       /* n+4 */
    unsigned L1v = floor_log2_uint64(tmp);   /* ⌊log₂(n+4)⌋ */
    idx_t tmp2 = tmp - L1v;                  /* n+4 - ⌊log₂(n+4)⌋ */
    unsigned b_n = floor_log2_uint64(tmp2);  /* ⌊log₂(n+4 - ⌊log₂(n+4)⌋)⌋ */

    /* binomial part: C(n+2-b_n, 2) */
    seq_t m = (seq_t)(n + 2 - b_n);
    seq_t binom = m * (m - 1) / 2;

    /* power part: 2(2^b_n - b_n) */
    seq_t pow2b = pow2(b_n);
    seq_t term = 2 * (pow2b - (seq_t)b_n);

    return binom + term;
}
