/*
 * A166074  a(n) = n^2 - [largest Fibonacci number <= n^2].
 * https://oeis.org/A166074
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n^2 - largest Fibonacci number ≤ n^2
 * Range:     u8 n <= 15, u16 n <= 431, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n ≥ 1 for the OEIS offset */
    if (n == 0) return 0;  /* not used, but harmless */

    seq_t square = (seq_t)n * (seq_t)n;

    /* Generate Fibonacci numbers until the next one would exceed square */
    seq_t fib0 = 1;   /* F(1) */
    seq_t fib1 = 1;   /* F(2) */
    seq_t largest = 1;  /* at least 1 because n ≥ 1 */

    while (1) {
        seq_t next = fib0 + fib1;  /* F(k+2) */
        if (next > square)
            break;
        largest = next;
        fib0 = fib1;
        fib1 = next;
    }

    return square - largest;
}
