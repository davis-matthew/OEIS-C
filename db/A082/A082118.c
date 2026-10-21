/*
 * A082118  Duplicate of A079344.
 * https://oeis.org/A082118
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = Fib(n) mod 8 (Pisano period 12)
 * Range:     i8 n <= 34, i16 n <= 34, i32 n <= 34, i64 n <= 34, i128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Precomputed Fibonacci numbers modulo 8 for one full period (length 12) */
    static const seq_t table[12] = {0, 1, 1, 2, 3, 5, 0, 5, 5, 2, 7, 1};
    /* Compute n modulo the period length (12) and look up the value */
    return table[n % 12];
}
