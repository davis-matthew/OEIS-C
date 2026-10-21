/*
 * A084558  a(0) = 0; for n >= 1: a(n) = largest m such that n >= m!.
 * https://oeis.org/A084558
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = largest m such that n >= m!
 * Range:     u8 n <= 1222, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Special case: 0! = 1, so a(0) = 0 by definition */
    if (n == 0)
        return (seq_t)0;

    seq_t n_val = (seq_t)n;
    seq_t fact = (seq_t)1;   /* factorial accumulator */
    seq_t m = (seq_t)1;      /* current factorial argument */

    while (fact <= n_val) {
        m += 1;
        fact *= m;          /* compute next factorial */
    }

    /* When the loop exits, fact > n_val so the previous m-1 is the answer */
    return m - 1;
}
