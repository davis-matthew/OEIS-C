/*
 * A220082  Numbers k such that 10*k-1 is a square.
 * https://oeis.org/A220082
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 162, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A220082
   Formula: a(n) = ((10*n - (-1)^n - 5)/2)^2 + 1) / 10
   Equivalent: let t = 5*n - 2 (n odd) or 5*n - 3 (n even), then a(n) = (t*t + 1)/10 */
seq_t a(idx_t n)
{
    seq_t t;
    if ((n & 1ULL) == 0)            /* n even */
        t = (seq_t)5 * (seq_t)n - 3;
    else                            /* n odd */
        t = (seq_t)5 * (seq_t)n - 2;
    return (t * t + 1) / 10;
}
