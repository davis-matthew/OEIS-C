/*
 * A097067  Expansion of g.f. (1-4*x+5*x^2)/(1-2*x)^2.
 * https://oeis.org/A097067
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 for n=0, * 0 for n=1, * (n-1) * 2^(n-2) for n >= 2. *
 *            This follows from the generating function (1-4x+5x^2)/(1-2x)^2
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 29, u64 n <= 60, u128 n <= 123,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }
    if (n == 1) {
        return (seq_t)0;
    }
    /* For n >= 2 */
    seq_t factor = (seq_t)(n - 1);          /* (n-1) in seq_t */
    seq_t pow2 = (seq_t)1 << (n - 2);       /* 2^(n-2) */
    return factor * pow2;
}
