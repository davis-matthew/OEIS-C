/*
 * A037543  Decimal expansion of a(n) is given by the first n terms of the
 *          periodic sequence with initial period 1,2,1.
 * https://oeis.org/A037543
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = floor((121/999)*10^n), i.e. the first n digits of the *
 *            repeating decimal 0.121121121..., or equivalently: digits of the
 *            * period 1,2,1 appended n times
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Treat n==0 as 0, although the sequence is defined for n>=1. */
    if (n == 0) return 0;

    seq_t val = 0;
    static const int pat[3] = {1, 2, 1};
    int pi = 0;                     /* current index into pat   */

    for (idx_t i = 0; i < n; ++i) {
        val = val * 10 + pat[pi];
        ++pi;
        if (pi == 3) pi = 0;
    }
    return val;
}
