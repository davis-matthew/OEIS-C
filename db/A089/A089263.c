/*
 * A089263  First differences of A080791.
 * https://oeis.org/A089263
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 9582, i16 n <= 9582, i32 n <= 9582, i64 n <= 9582,
 *            i128 n <= 9582, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A089263 – First differences of A080791
 *
 *  Recurrence (OEIS definition):
 *      a(0) = 0
 *      a(1) = 0
 *      a(2n)   = a(n) + 1
 *      a(2n+1) = -1   (for n > 0)
 *
 *  Equivalently: a(n) is the number of trailing zeros of n in binary,
 *  minus one if n is not a power of two.  This implementation uses the
 *  recurrence, which requires only O(log n) recursion steps and
 *  constant‑time bit operations, well within the time limits.
 *
 *  Note: n = 0 or 1 are handled directly.  For any even n >= 2 the
 *  value is a(n/2) + 1, otherwise for odd n > 1 the value is -1.
 */

seq_t a(idx_t n) {
    /* Base cases: a(0) = a(1) = 0 */
    if (n <= 1)
        return 0;

    /* For odd n (n > 1) the value is always -1 */
    if ((n & 1) != 0)
        return (seq_t)-1;

    /* Even n ≥ 2 : a(n) = a(n/2) + 1 */
    return a(n / 2) + 1;
}
