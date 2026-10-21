/*
 * A079757  Periodic sequence 1, 0, -2, 3, -2, 0, ...
 * https://oeis.org/A079757
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = a(n-6); a(0)=1, a(1)=0, a(2)=-2, a(3)=3, a(4)=-2, a(5)=0
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t r = n % 6;
    switch (r) {
        case 0: return (seq_t)1;
        case 1: return (seq_t)0;
        case 2: return (seq_t)-2;
        case 3: return (seq_t)3;
        case 4: return (seq_t)-2;
        case 5: return (seq_t)0;
    }
    /* unreachable, but satisfies the compiler */
    return (seq_t)0;
}
