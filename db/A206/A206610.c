/*
 * A206610  Fibonacci sequence beginning 13, 8.
 * https://oeis.org/A206610
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2); a(1) = 13, a(2) = 8
 * Range:     u8 n <= 8, u16 n <= 19, u32 n <= 42, u64 n <= 89, u128 n <= 181,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first two terms explicitly */
    if (n == 1) return 13;
    if (n == 2) return 8;

    /* Iterate the recurrence a(n) = a(n-1) + a(n-2) up to n. */
    seq_t prev = 13;   /* a(1) */
    seq_t cur  = 8;    /* a(2) */
    seq_t next;

    for (idx_t i = 3; i <= n; ++i) {
        next = prev + cur;   /* a(i) = a(i-1) + a(i-2) */
        prev = cur;
        cur  = next;
    }
    return cur;   /* cur holds a(n) after the loop */
}
