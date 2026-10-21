/*
 * A235715  a(n) = number of times (n-1) occurs in the fundamental period of
 *          Fibonacci numbers modulo n.
 * https://oeis.org/A235715
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of times (n‑1) occurs in the Pisano period of
 *            Fibonacci numbers modulo n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return 1;

    idx_t f0 = 0;
    idx_t f1 = 1 % n;          /* f1 < n (1 for n > 1) */
    seq_t count = 0;

    /* count the first term if needed (only n=1 case, already handled) */
    if (f0 == n - 1)
        ++count;

    while (1) {
        idx_t next = (f0 + f1) % n;
        f0 = f1;
        f1 = next;
        /* stop when a new period starts: (0,1) */
        if (f0 == 0 && f1 == 1)
            break;
        if (f0 == n - 1)
            ++count;
    }

    return count;
}
