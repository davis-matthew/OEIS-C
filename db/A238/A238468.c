/*
 * A238468  Period 7: repeat [0, 0, -1, 1, -1, 1, 0].
 * https://oeis.org/A238468
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = periodic sequence with period 7: a(0)=0, a(1)=0, a(2)=-1,
 *            a(3)=1, a(4)=-1, a(5)=1, a(6)=0, and a(n+7)=a(n)
 * Range:     i8 n <= 90, i16 n <= 90, i32 n <= 90, i64 n <= 90, i128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t r = n % 7;
    switch (r)
    {
        case 0: return 0;
        case 1: return 0;
        case 2: return -1;
        case 3: return 1;
        case 4: return -1;
        case 5: return 1;
        case 6: return 0;
    }
    /* Unreachable, but keep compiler happy */
    return 0;
}
