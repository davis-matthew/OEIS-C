/*
 * A008651  Molien series of binary icosahedral group.
 * https://oeis.org/A008651
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 - n + floor(n/2) + floor(n/3) + floor(n/5)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This follows the OEIS description of the Molien series for the binary icosahedral group. */
seq_t a(idx_t n)
{
    /* Convert n to a wider unsigned type to avoid overflow in intermediate calculations. */
    unsigned long long nn = (unsigned long long)n;
    unsigned long long res = 1 + nn/2 + nn/3 + nn/5 - nn;
    return (seq_t)res;
}
