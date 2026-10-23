/*
 * A131201  Period 24: repeat [0, 1, 2, 5, 2, 9, 0, 9, 8, 5, 8, 1, 0, -1, -2,
 *          -5, -2, -9, 0, -9, -8, -5, -8, -1].
 * https://oeis.org/A131201
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, less, easy
 * Formula:   not stated
 * Range:     i8 n <= 95, i16 n <= 95, i32 n <= 95, i64 n <= 95, i128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A131201: Periodic sequence with period 24
 *  Sequence values: 0, 1, 2, 5, 2, 9, 0, 9, 8, 5, 8, 1, 0, -1, -2, -5,
 *  -2, -9, 0, -9, -8, -5, -8, -1
 *  The sequence is defined for all n >= 0 as a(n) = period[n mod 24].
 *  This implementation uses a simple lookup table and modular arithmetic,
 *  ensuring O(1) time per call and constant space.
 */

seq_t a(idx_t n)
{
    /* Each entry must be compatible with seq_t (signed or arbitrary-precision). */
    static const seq_t period[24] = {
        (seq_t)0,  (seq_t)1,  (seq_t)2,  (seq_t)5,  (seq_t)2,  (seq_t)9,
        (seq_t)0,  (seq_t)9,  (seq_t)8,  (seq_t)5,  (seq_t)8,  (seq_t)1,
        (seq_t)0,  (seq_t)-1, (seq_t)-2, (seq_t)-5, (seq_t)-2, (seq_t)-9,
        (seq_t)0,  (seq_t)-9, (seq_t)-8, (seq_t)-5, (seq_t)-8, (seq_t)-1
    };
    return period[n % 24];
}
