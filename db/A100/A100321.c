/*
 * A100321  The trinomial transform (A027907) gives powers of 2, while the
 *          trinomial transform of this sequence shift one place left gives
 *          powers of 3.
 * https://oeis.org/A100321
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = -2*a(n-1) + 2*a(n-2) + 3*a(n-3) - 2*a(n-4), initial
 *            values: a(0)=1, a(1)=1, a(2)=0, a(3)=2
 * Range:     i8 n <= 8, i16 n <= 16, i32 n <= 32, i64 n <= 64, i128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 1;
    if (n == 2) return 0;
    if (n == 3) return 2;

    seq_t a0 = 1;
    seq_t a1 = 1;
    seq_t a2 = 0;
    seq_t a3 = 2;
    seq_t an;

    for (idx_t i = 4; i <= n; ++i) {
        an = -2 * a3 + 2 * a2 + 3 * a1 - 2 * a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }
    return a3;
}
