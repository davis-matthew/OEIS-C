/*
 * A070931  Numbers k such that the smallest integer value >= 0 of the form
 *          x^3 - k^2 equals the smallest integer value >= 0 of the form x^2 -
 *          k^3.
 * https://oeis.org/A070931
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-2)^6 for n >= 5; special cases: a(1)=1, a(2)=64,
 *            a(3)=68, a(4)=120
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 24, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the first four exceptional terms */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)64;
    if (n == 3) return (seq_t)68;
    if (n == 4) return (seq_t)120;

    /* n >= 5: compute (n-2)^6 using only seq_t arithmetic */
    seq_t base = (seq_t)(n - 2);      /* n-2 fits in idx_t; cast to seq_t */
    seq_t sq   = base * base;          /* (n-2)^2 */
    seq_t q    = sq * sq;              /* (n-2)^4 */
    return q * sq;                    /* (n-2)^6 */
}
