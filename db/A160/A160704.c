/*
 * A160704  Jacobsthal sequence A001045 convolved with A139251 (first
 *          differences of toothpick numbers).
 * https://oeis.org/A160704
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 21, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   _compute_d
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/_compute_d.h"
/*  OEIS A160704
 *
 *  a(n) = 2*a(n-1) + d(n)    (n ≥ 2,  a(1) = 1)
 *
 *  d(n) – sequence A160552 (first differences of toothpick numbers) is
 *  defined recursively:
 *        d(1) = 1
 *        if n is odd:      d(n) = 2*d((n-1)/2) + 1
 *        if n is even:
 *               let m = n/2
 *               if m is even: d(n) = d(m)
 *               else         : d(n) = d(m) + 2*d(m-1)
 *
 *  The implementation evaluates d(n) on demand and then iteratively
 *  builds a(n).  It works for any unsigned integer type `seq_t` and any
 *  index type `idx_t` (uint64_t).  All arithmetic is performed in
 *  `seq_t`, with only the allowed operators and casts.  No global state
 *  is used, so calls may arrive in any order.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* outside the offset */
    seq_t acc = (seq_t)1;                 /* a(1) = 1 */
    for (idx_t i = 2; i <= n; ++i) {
        seq_t d = _compute_d(i);
        acc = ((seq_t)2) * acc + d;
    }
    return acc;
}
