/*
 * A087257  Duplicate of A033496.
 * https://oeis.org/A087257
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   not stated
 * Range:     i8 n <= 22, i16 n <= 22, i32 n <= 22, i64 n <= 22, i128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A087257 (duplicate of A033496):
   a(0)=1, a(1)=2, a(2)=4, a(3)=8, a(4)=16, a(5)=20, a(6)=24,
   a(7)=32, a(8)=40.
   For n>=9, a(n) = a(n-1) + ( ((n-8) % 3) == 1 ? 8 : 4 ). */
seq_t a(idx_t n) {
    if (n == 0u) return 1;
    if (n == 1u) return 2;
    if (n == 2u) return 4;
    if (n == 3u) return 8;
    if (n == 4u) return 16;
    if (n == 5u) return 20;
    if (n == 6u) return 24;
    if (n == 7u) return 32;
    if (n == 8u) return 40;

    seq_t val = 40;
    for (idx_t i = 9; i <= n; ++i) {
        val += (((i - 8) % 3) == 1 ? (seq_t)8 : (seq_t)4);
    }
    return val;
}
