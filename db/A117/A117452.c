/*
 * A117452  Periodic {2, -1, 1, 0, 0} - 0^n.
 * https://oeis.org/A117452
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = 1 if n = 0; otherwise a(n) = 2 for n%5 = 0, a(n) = -1 for
 *            n%5 = 1, a(n) = 1 for n%5 = 2, a(n) = 0 for n%5 = 3 or 4
 * Range:     i8 n <= 98, i16 n <= 98, i32 n <= 98, i64 n <= 98, i128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    idx_t r = n % 5;
    switch (r) {
        case 0: return (seq_t)2;
        case 1: return (seq_t)-1;
        case 2: return (seq_t)1;
        default: return (seq_t)0;
    }
}
