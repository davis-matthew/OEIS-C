/*
 * A132367  Period 6: repeat [1, 1, 2, -1, -1, -2].
 * https://oeis.org/A132367
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, less
 * Formula:   a(n) = periodic sequence [1, 1, 2, -1, -1, -2] repeating every
 *            6. Recurrence: a(n) + a(n-3) = 0 for n > 2; initial terms:
 *            a(0)=1, a(1)=1, a(2)=2
 * Range:     i8 n <= 101, i16 n <= 101, i32 n <= 101, i64 n <= 101,
 *            i128 n <= 101, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use the period to compute the value directly. */
    switch ((unsigned)n % 6U) {
        case 0: return 1;
        case 1: return 1;
        case 2: return 2;
        case 3: return -1;
        case 4: return -1;
        case 5: return -2;
        /* The default branch is unreachable for unsigned n % 6,
           but the compiler may warn about missing return if not present. */
        default: return 0;
    }
}
