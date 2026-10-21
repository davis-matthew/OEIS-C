/*
 * A217290  Integers n such that 2*cos(2*Pi/n) is an integer.
 * https://oeis.org/A217290
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, fini, full
 * Formula:   a(n) = the nth element of the sorted list of integers n such
 *            that 2*cos(2π/n) is integer, namely -6,-4,-3,-2,-1,1,2,3,4,6
 * Range:     i8 n <= 9, i16 n <= 9, i32 n <= 9, i64 n <= 9, i128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t arr[] = { -6, -4, -3, -2, -1, 1, 2, 3, 4, 6 };
    if (n < sizeof(arr) / sizeof(arr[0]))
        return arr[n];
    else
        return 0; /* indices beyond the defined sequence return 0 */
}
