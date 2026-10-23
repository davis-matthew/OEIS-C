/*
 * A176892  Decimal representation of the reverted binary representation of n
 *          followed by digits substitution 0->2, 1->3.
 * https://oeis.org/A176892
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = decimal number obtained from the reversed binary
 *            representation of n, with digits 0→2 and 1→3 (a(0)=2)
 * Range:     u8 n <= 4, u16 n <= 31, u32 n <= 1023, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#pragma once
seq_t a(idx_t n)
{
    /* Handle the special case n==0 first. */
    if (n == 0)
        return (seq_t)2;

    seq_t result = 0;
    idx_t x = n;
    while (x != 0) {
        /* Map the least‑significant bit to the corresponding digit. */
        seq_t digit = ((x & 1) ? (seq_t)3 : (seq_t)2);
        result = result * (seq_t)10 + digit;
        x >>= 1;
    }
    return result;
}
