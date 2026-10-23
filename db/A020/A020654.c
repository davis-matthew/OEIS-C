/*
 * A020654  Lexicographically earliest infinite increasing sequence of
 *          nonnegative numbers containing no 5-term arithmetic progression.
 * https://oeis.org/A020654
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = interpret the base‑4 representation of (n-1) as a base‑5
 *            number
 * Range:     u8 n <= 133, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* using n-1 because the sequence starts with a(1)=0                      */
    idx_t m = n - 1;
    seq_t result = 0;
    seq_t power  = 1;          /* current 5^i value                                   */

    while (m > 0)              /* process base‑4 digits, least significant first   */
    {
        int digit = m & 3;     /* m % 4 (fast for unsigned)                         */
        result += (seq_t)digit * power;   /* add digit * 5^i                         */
        m >>= 2;              /* equivalent to m /= 4                             */
        power *= 5;           /* move to next power of 5                          */
    }

    return result;
}
