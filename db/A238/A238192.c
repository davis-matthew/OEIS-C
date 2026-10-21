/*
 * A238192  In the Collatz (3x+1) iteration of n, the last odd number before
 *          1, or 0 if there is no such number.
 * https://oeis.org/A238192
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the last odd number encountered in the Collatz (3x+1)
 *            iteration of n before reaching 1; if no odd number occurs (i.e.,
 *            n = 1 or n power of 2), a(n) = 0
 * Range:     u8 n <= 74, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Ensure we start with the proper type. */
    seq_t cur = (seq_t)n;
    seq_t last_odd = 0;

    while (cur != 1) {
        /* If cur is odd, record it and apply the 3x+1 rule. */
        if ((cur & 1) != 0) {
            last_odd = cur;
            cur = cur * 3 + 1;
        } else {
            /* If cur is even, halve it. */
            cur = cur >> 1;
        }
    }
    return last_odd;
}
