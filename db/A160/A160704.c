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
 * Validated: partial, pass, 21 terms through n = 21 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t _compute_d(idx_t n)
{
    /* base cases – valid for all n ≥ 1 */
    if (n == 1) return (seq_t)1;
    if (n == 0) return (seq_t)0;          /* safety for n == 0 */

    if (n & 1ULL) {                       /* n odd */
        return ((seq_t)2) * _compute_d((n - 1) / 2) + (seq_t)1;
    } else {                              /* n even */
        idx_t m = n >> 1;                 /* m = n/2 */
        if ((m & 1ULL) == 0) {           /* m even */
            return _compute_d(m);
        } else {                         /* m odd */
            seq_t dm  = _compute_d(m);
            seq_t dm1 = _compute_d(m - 1);
            return dm + ((seq_t)2) * dm1;
        }
    }
}
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
