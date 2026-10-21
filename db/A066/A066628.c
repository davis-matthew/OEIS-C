/*
 * A066628  a(n) = n - the largest Fibonacci number <= n.
 * https://oeis.org/A066628
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n - largest Fibonacci number <= n
 * Range:     u8 n <= 264, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tn = (seq_t)n;          /* convert index to seq_t for arithmetic */
    seq_t prev = 0;
    seq_t curr = 1;
    seq_t largest = 0;

    /* Find the largest Fibonacci number not exceeding tn */
    while (curr <= tn)
    {
        largest = curr;
        seq_t next = prev + curr;
        prev = curr;
        curr = next;
    }

    return tn - largest;
}
