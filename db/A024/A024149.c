/*
 * A024149  a(n) = 12^n - n^9.
 * https://oeis.org/A024149
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 12^n - n^9
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 8, i64 n <= 17, i128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t pow12 = 1;
    seq_t base12 = 12;
    idx_t exp = n;
    while (exp)
    {
        if (exp & 1)
            pow12 *= base12;
        exp >>= 1;
        if (exp)
            base12 *= base12;
    }

    seq_t nn = (seq_t)n;
    seq_t pown9 = 1;
    for (int i = 0; i < 9; ++i)
        pown9 *= nn;

    return pow12 - pown9;
}
