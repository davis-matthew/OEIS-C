/*
 * A055964  n + reversal of hexadecimal (base 16) digits of n (written in base
 *          10).
 * https://oeis.org/A055964
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn, look
 * Formula:   a(n) = n + A056962(n) – the decimal value of the hexadecimal
 *            digits of n reversed
 * Range:     u8 n <= 30, u16 n <= 4110, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t rev = 0;               /* reversed hexadecimal digits (in base 16) */
    uint64_t t   = n;            /* local copy of n to iterate over hex digits */

    while (t != 0) {
        uint64_t digit = t & 0xF;        /* least significant hex digit */
        rev = (rev << 4) + digit;      /* append digit to reversed value */
        t >>= 4;
    }

    return (seq_t)n + rev;          /* n + reversed hex value */
}
