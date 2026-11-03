/*
 * A096036  a(n) = ceiling(n/a(ceiling(n/2))); a(1) = 1.
 * https://oeis.org/A096036
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Validated: partial, pass, 91 terms through n = 91 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t _a_rec(idx_t n)
{
    if (n == (idx_t)1)                 /* base case */
        return (seq_t)1;

    idx_t m = (n + (idx_t)1) / (idx_t)2;   /* ceil(n/2)  */
    seq_t denom = _a_rec(m);

    seq_t numer = (seq_t)n;
    /* ceil(numer/denom) = (numer + denom - 1) / denom */
    return (numer + denom - (seq_t)1) / denom;
}
/* Recursive formula: a(n) = ceil( n / a( ceil(n/2) ) ) ,  a(1) = 1 */
/* The implementation is fully recursive, making one call per distinct
   argument.  All arithmetic is performed with the seq_t type, using only
   the allowed operators (+, -, *, /, %) and conversion from integer
   constants. */

extern seq_t a(idx_t n)
{
    return _a_rec(n);
}
