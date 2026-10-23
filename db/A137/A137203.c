/*
 * A137203  Number of Fibonacci numbers less than or equal to n^2.
 * https://oeis.org/A137203
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of Fibonacci numbers <= n^2
 * Range:     u8 n <= 15, u16 n <= 1454, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes by iterating the Fibonacci sequence until exceeding n^2.
   Each Fibonacci term (including the duplicate 1 and the initial 0)
   is counted when it is <= n^2. */
seq_t a(idx_t n)
{
    /* compute n^2 in seq_t */
    seq_t limit = (seq_t)n * (seq_t)n;

    /* first two Fibonacci numbers: F0 = 0, F1 = 1 */
    seq_t f0 = (seq_t)0;
    seq_t f1 = (seq_t)1;

    seq_t count = (seq_t)0;

    while (f0 <= limit) {
        count++;
        seq_t next = f0 + f1;
        f0 = f1;
        f1 = next;
    }

    return count;
}
