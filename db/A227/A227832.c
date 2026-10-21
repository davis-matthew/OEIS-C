/*
 * A227832  Sum of odd numbers starting with 1, alternating signs (beginning
 *          with plus).
 * https://oeis.org/A227832
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = n * (-1)^n + 2
 * Range:     i8 n <= 125, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(1) = 1, a(2) = 4, a(3) = -1, ... */
    if (n == 0)
        return 0;            /* undefined, never called */
    seq_t nn = (seq_t)n;
    if (n & 1)                /* odd n: -n + 2 */
        return -nn + (seq_t)2;
    else                      /* even n:  n + 2 */
        return nn + (seq_t)2;
}
