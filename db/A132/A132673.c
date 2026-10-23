/*
 * A132673  a(1)=1, a(n) = 9*a(n-1) if the minimal positive integer number not
 *          yet in the sequence is greater than a(n-1), else a(n) = a(n-1) -
 *          1.
 * https://oeis.org/A132673
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; for n > 1, a(n) = 9*a(n-1) if the smallest positive
 *            integer not yet in the sequence is greater than a(n-1);
 *            otherwise a(n) = a(n-1) - 1. This implementation follows the
 *            recurrence directly, keeping track of the current value, the
 *            maximum value visited so far, and the minimal missing int...
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    seq_t cur = (seq_t)1;          /* current value a(k) */
    seq_t max_visited = (seq_t)1;  /* largest value seen so far (visited set is 1..max_visited) */
    seq_t missing = (seq_t)2;      /* smallest positive integer not yet seen */

    for (idx_t i = 2; i <= n; ++i)
    {
        if (missing > cur)          /* multiply rule */
        {
            cur = cur * (seq_t)9;
            if (cur > max_visited)
                max_visited = cur;
        }
        else                        /* decrement rule */
        {
            cur = cur - (seq_t)1;
        }

        /* If we have just visited the previously missing integer,
           advance missing to the next unseen integer.
           At this point the visited set is 1..max_visited. */
        if (cur == missing)
            missing = max_visited + (seq_t)1;
    }
    return cur;
}
