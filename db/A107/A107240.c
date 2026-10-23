/*
 * A107240  Sum of squares of first n tribonacci numbers (A000213).
 * https://oeis.org/A107240
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{i=1..n} A000213(i)^2 (sum of squares of the
 *            Tribonacci numbers)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 20, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Tribonacci seeds: T1 = 1, T2 = 1, T3 = 1 */
    seq_t t[3] = {1, 1, 1};
    seq_t sum = 0;

    if (n == 0)    /* undefined for the OEIS offset; return 0 for safety */
        return sum;

    /* handle first three terms explicitly */
    sum = t[0] * t[0];          /* i = 1 */
    if (n == 1)
        return sum;

    sum += t[1] * t[1];          /* i = 2 */
    if (n == 2)
        return sum;

    sum += t[2] * t[2];          /* i = 3 */
    if (n == 3)
        return sum;

    /* compute next Tribonacci numbers and accumulate the sum of squares */
    for (idx_t i = 4; i <= n; ++i) {
        seq_t next = t[0] + t[1] + t[2];
        /* shift the window */
        t[0] = t[1];
        t[1] = t[2];
        t[2] = next;
        sum += next * next;
    }
    return sum;
}
