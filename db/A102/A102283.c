/*
 * A102283  Period 3: repeat [0, 1, -1].
 * https://oeis.org/A102283
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, mult
 * Formula:   a(n) = {0 if n mod 3 = 0, 1 if n mod 3 = 1, -1 if n mod 3 = 2}
 * Range:     i8 n <= 104, i16 n <= 104, i32 n <= 104, i64 n <= 104,
 *            i128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    switch (n % 3U) {
        case 0: return (seq_t)0;
        case 1: return (seq_t)1;
        default: return (seq_t)-1;
    }
}
