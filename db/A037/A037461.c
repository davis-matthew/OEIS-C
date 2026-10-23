/*
 * A037461  a(n)=Sum{d(i)*7^i: i=0,1,...,m}, where Sum{d(i)*4^i: i=0,1,...,m}
 *          is the base 4 representation of n.
 * https://oeis.org/A037461
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = ∑_{i=0}^{m} d(i)*7^i, where {d(i)} are the base‑4 digits
 *            of n. This interprets the base‑4 representation of n as a number
 *            in base‑7
 * Range:     u8 n <= 63, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;          /* accumulated value in base‑7 */
    seq_t pow    = 1;          /* current power of 7 (7^i) */
    idx_t temp   = n;          /* working copy of n */

    while (temp != 0)
    {
        uint8_t digit = temp % 4;          /* next base‑4 digit (0..3) */
        result += (seq_t)digit * pow;     /* add digit*7^i to the result */
        pow   *= (seq_t)7;                /* advance to the next power of 7 */
        temp  /= 4;                       /* shift to the next base‑4 digit */
    }
    return result;
}
