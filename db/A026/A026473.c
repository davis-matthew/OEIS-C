/*
 * A026473  a(n) = least positive integer > a(n-1) and not a(i)+a(j)+a(k) for
 *          1<=i<j<k<=n.
 * https://oeis.org/A026473
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1,2,4,5,6,14,16,18,30,42,43,44,56,69,70 for n ≤ 15; for n
 *            ≥ 16: a(n) = (13*n - 44)/2 if n is even, a(n) = (13*n - 55)/2 if
 *            n is odd. This gives the least positive integer > a(n-1) that
 *            cannot be expressed as a(i)+a(j)+a(k) with 1≤i<j<k≤n
 * Range:     u8 n <= 23, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* first 15 terms, indexed from 1 */
    static const seq_t first[15] = {
        1, 2, 4, 5, 6,      /* n = 1..5 */
        14, 16, 18, 30, 42, /* n = 6..10 */
        43, 44, 56, 69, 70  /* n = 11..15 */
    };

    if (n <= 15)
        return first[n - 1];

    /* n ≥ 16 */
    seq_t result;
    if ((n & 1) == 0) {          /* even n */
        result = (seq_t)13 * n;
        result -= 44;
        result /= 2;
    } else {                     /* odd n */
        result = (seq_t)13 * n;
        result -= 55;
        result /= 2;
    }
    return result;
}
