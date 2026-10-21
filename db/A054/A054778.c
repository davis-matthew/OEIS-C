/*
 * A054778  a(n) = 5n*(5n-1)*(5n-2)*(5n-3)*(5n-4).
 * https://oeis.org/A054778
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 5n*(5n-1)*(5n-2)*(5n-3)*(5n-4)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 17, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A054778 - Falling factorial of five consecutive integers starting at 5n-4 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t five_n = (seq_t)n * 5;          /* 5n */
    seq_t res   = five_n * (five_n - 1);  /* 5n*(5n-1)          */
    res        *= (five_n - 2);           /* * (5n-2)           */
    res        *= (five_n - 3);           /* * (5n-3)           */
    res        *= (five_n - 4);           /* * (5n-4)           */
    return res;
}
