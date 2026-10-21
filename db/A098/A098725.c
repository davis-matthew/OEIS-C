/*
 * A098725  a(4*n) = 0, a(2*n+1) = 1, a(4*n+2) = a(n+1).
 * https://oeis.org/A098725
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   recursion a(4*n) = 0 a(2*n+1) = 1 a(4*n+2) = a(n+1) a(0) = 0,
 *            a(1) = 1,
 * Range:     u8 n <= 8920, u16 n <= 8920, u32 n <= 8920, u64 n <= 8920,
 *            u128 n <= 8920, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* iterative reduction using the recurrence rules */
    while (1)
    {
        /* divisible by 4 -> 0 */
        if ((n & 3) == 0)
            return 0;

        /* odd -> 1 */
        if (n & 1)
            return 1;

        /* n ≡ 2 (mod 4): n = 4*m + 2  →  a(n) = a(m + 1) */
        n = (n - 2) / 4 + 1;
    }
}
