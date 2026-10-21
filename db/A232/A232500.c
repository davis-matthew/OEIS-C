/*
 * A232500  Oscillating orbitals over n sectors (nonpositive values indicating
 *          there exist none).
 * https://oeis.org/A232500
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, nice, changed
 * Formula:   a(n) = (n!/(k!^2)) * ((k-1)/(k+1)), where k = floor(n/2)
 * Range:     i8 n <= 5, i16 n <= 11, i32 n <= 25, i64 n <= 34, i128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses the simple recurrence:
 *   a(0) = -1, a(1) = -1; a(2) = 0, a(3) = 0, a(4) = 2
 *   For n ≥ 5:
 *     if n is odd   → a(n) = a(n-1) * n
 *     if n is even  → a(n) = a(n-1) * 4 * (n-2) / ((n-4)*(n+2))   */

seq_t a(idx_t n)
{
    /* Handle the small indices directly */
    if (n == 0) return -1;
    if (n == 1) return -1;
    if (n == 2) return 0;
    if (n == 3) return 0;
    if (n == 4) return 2;

    seq_t val = 2;          /* a(4) */
    for (idx_t i = 5; i <= n; ++i) {
        if (i & 1U) {       /* odd n */
            val = val * (seq_t)i;
        } else {            /* even n */
            seq_t numerator   = val * 4 * (seq_t)(i - 2);
            seq_t denominator = (seq_t)(i - 4) * (seq_t)(i + 2);
            val = numerator / denominator;
        }
    }
    return val;
}
