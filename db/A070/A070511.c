/*
 * A070511  Duplicate of A070431.
 * https://oeis.org/A070511
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = {0, 1, 4, 3, 4, 1}[n mod 6]
 * Range:     i8 n <= 100, i16 n <= 100, i32 n <= 100, i64 n <= 100,
 *            i128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t m = n % 6;
    switch (m) {
        case 0: return (seq_t)0;
        case 1: return (seq_t)1;
        case 2: return (seq_t)4;
        case 3: return (seq_t)3;
        case 4: return (seq_t)4;
        case 5: return (seq_t)1;
    }
    /* Unreachable, but keep compiler happy */
    return (seq_t)0;
}
