/*
 * A082894  a(n) is the closest number to 2^n which is divisible by n.
 * https://oeis.org/A082894
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*floor((floor(n/2)+2^n)/n)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* OEIS A082894:  closest number to 2^n that is divisible by n */
    if (n == 0)
        return (seq_t)0;              /* undefined for n=0, but return 0 safely */

    /* 2^n as a seq_t.  For builtin widths this may overflow for large n,
       but the sequence is only required to be meaningful when the
       resulting value fits in seq_t (or seq_t is an arbitrary‑precision
       type).  The left shift operator handles the required
       arbitrary‑precision arithmetic when seq_t is a big integer type. */
    seq_t pow2 = ((seq_t)1) << n;

    /* floor(n/2) */
    seq_t half = (seq_t)(n / 2);

    /* (2^n + floor(n/2)) / n  (integer division, rounding down) */
    seq_t q = (pow2 + half) / (seq_t)n;

    /* multiply back by n to obtain a multiple of n */
    return q * (seq_t)n;
}
