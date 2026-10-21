/*
 * A087698  Triangle read by rows, giving T(n,k) = maximum number of examples
 *          (Boolean inputs) at Hamming distance 2 for symmetric Boolean
 *          functions that can have different outputs.
 * https://oeis.org/A087698
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A087698 – triangle read by rows.
 *
 * The triangle entry T(r,k) (0‑based row r, 0‑≤ k ≤ r) is given by
 *     T(r,k) = C(r,k) - 2*C(r-1,r-k-1) + 2*C(r-2,r-k-2)
 * for all r,k except the single exceptional case T(1,0)=1.
 * (Here C(n,k) is the binomial coefficient, with C(n,k)=0 if k>n.)
 *
 * The sequence a(n) is the triangle read in row order, i.e.
 * a(r*(r+1)/2 + k) = T(r,k).
 */
#include <stdbool.h>
#include "../utils/binomial.h"   /* provides: static seq_t binomial(idx_t n, idx_t k) */

/* Return C(n,k); if k>n then 0.  All parameters are unsigned, so underflow
 * yields a very large number but the > comparison blocks the call. */
static inline seq_t safe_binomial(idx_t n, idx_t k)
{
    return (k > n) ? 0 : binomial(n, k);
}

/* a(n) – returns the nth term of A087698. */
seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2. */
    idx_t r = 0;
    while (true) {
        idx_t next = (r + 1) * (r + 2) / 2;
        if (n < next) break;
        ++r;
    }
    idx_t k = n - r * (r + 1) / 2;

    /* Special cases according to the OEIS table. */
    if (r == 0) return (seq_t)1;          /* T(0,0) */
    if (r == 1 && k == 0) return (seq_t)1;/* T(1,0) */

    /* Compute the three binomial terms that appear in the formula. */
    seq_t c1 = safe_binomial(r, k);
    seq_t c2 = 0, c3 = 0;

    /* C(r-1, r-k-1) is defined only when k ≤ r-1. */
    if (k <= r - 1) c2 = safe_binomial(r - 1, r - k - 1);

    /* C(r-2, r-k-2) is defined only when r ≥ 2 and k ≤ r-2. */
    if (r >= 2 && k <= r - 2) c3 = safe_binomial(r - 2, r - k - 2);

    /* Apply the formula T(r,k).  All arithmetic is on unsigned seq_t. */
    seq_t res = c1;
    res -= (seq_t)2 * c2;
    res += (seq_t)2 * c3;
    return res;
}
