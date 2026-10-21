/*
 * A053839  a(n) = (sum of digits of n written in base 4) modulo 4.
 * https://oeis.org/A053839
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = (sum of digits of n in base 4) mod 4
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;            /* running sum of base‑4 digits */
    idx_t  x   = n;           /* work on a copy of the index */

    /* Since 4 is a power of two, n % 4 == n & 3
       and n / 4 == n >> 2 for unsigned integers. */
    while (x != 0) {
        sum += (x & 3);       /* add the least significant base‑4 digit */
        x >>= 2;              /* shift to process the next digit */
    }

    return sum & 3;          /* take the result modulo 4 */
}
