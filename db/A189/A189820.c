/*
 * A189820  a(3*k-2) = a(k), a(3*k-1) = a(k), a(3*k) = 1 for k >= 1, starting
 *          with a(1) = 0.
 * https://oeis.org/A189820
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 if the base‑3 representation of (n-1) contains a digit
 *            2, otherwise a(n) = 0. Equivalent recursive definition: a(3*k-2)
 *            = a(k), a(3*k-1) = a(k), a(3*k) = 1 (k ≥ 1)
 * Range:     u8 n <= 9293, u16 n <= 9293, u32 n <= 9293, u64 n <= 9293,
 *            u128 n <= 9293, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n ≥ 1 by definition of the sequence. */
    idx_t m = n - 1;                 /* work with n-1 */
    while (m) {
        if ((m % 3) == 2)            /* digit 2 found */
            return (seq_t)1;
        m /= 3;
    }
    return (seq_t)0;                 /* no digit 2 */
}
