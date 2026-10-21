/*
 * A085952  First n-digit number that occurs in the sequence A085951.
 * https://oeis.org/A085952
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(0) = 1 , a(n) = 9*10^n + 1 for n > 0 Recurrence used for n ≥
 *            3: a(n) = 11*a(n-1) - 10*a(n-2) Initial values: a(0) = 1, a(1) =
 *            91, a(2) = 901
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle small n directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)91;
    if (n == 2) return (seq_t)901;

    /* For n ≥ 3 use the recurrence relation.
       Note: prev1 holds a(n-1), prev2 holds a(n-2) */
    seq_t prev2 = (seq_t)91;   /* a(1) */
    seq_t prev1 = (seq_t)901;  /* a(2) */
    seq_t cur;

    for (idx_t i = 3; i <= n; ++i) {
        cur = prev1 * (seq_t)11 - prev2 * (seq_t)10;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
