/*
 * A163201  Alternating sum of the cubes of the first n even-indexed Fibonacci
 *          numbers.
 * https://oeis.org/A163201
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0) = 0 a(1) = -1 a(2) = 26 a(3) = -486 a(4) = 8775 For n ≥ 5:
 *            a(n) = -20*a(n-1) - 35*a(n-2) + 35*a(n-3) + 20*a(n-4) + a(n-5)
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 8, i64 n <= 15, i128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: idx_t is an unsigned integer type (uint64_t).  seq_t may be
   a primitive signed integer type (int8_t…__int128) or an arbitrary‑
   precision integer class that overloads the usual arithmetic
   operators.  The implementation below uses only those operators and
   therefore works both in C11 and C++17.  No dynamic allocation or
   caching is required – the recurrence depends only on the previous
   five terms.  The function is O(n) in time and O(1) in memory.  */

seq_t a(idx_t n)
{
    /* Handle the first five terms directly.  */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)-1;
    if (n == 2) return (seq_t)26;
    if (n == 3) return (seq_t)-486;
    if (n == 4) return (seq_t)8775;

    /* Variables hold the last five terms:
       a0 = a(n-5), a1 = a(n-4), a2 = a(n-3),
       a3 = a(n-2), a4 = a(n-1).  */
    seq_t a0 = (seq_t)0;
    seq_t a1 = (seq_t)-1;
    seq_t a2 = (seq_t)26;
    seq_t a3 = (seq_t)-486;
    seq_t a4 = (seq_t)8775;

    /* Iterate from i = 5 up to n, updating the sliding window.  */
    for (idx_t i = 5; i <= n; ++i)
    {
        seq_t cur = - (seq_t)20 * a4
                 - (seq_t)35 * a3
                 + (seq_t)35 * a2
                 + (seq_t)20 * a1
                 + (seq_t)1  * a0;

        /* Shift the window: drop a0, shift others up, and bring cur as a4. */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = cur;
    }

    return a4;
}
