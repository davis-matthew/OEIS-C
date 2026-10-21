/*
 * A090617  Exponent of highest power of 8 dividing n!.
 * https://oeis.org/A090617
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor( ( floor(n/2) + floor(n/4) + floor(n/8) + … ) / 3 )
 * Range:     u8 n <= 274, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This is the exponent of the highest power of 8 dividing n! (v8(n!)). */
seq_t a(idx_t n)
{
    seq_t sum = 0;
    /* Legendre's formula for v2(n!) */
    while (n > 0) {
        n >>= 1;        /* n = floor(n/2) */
        sum += (seq_t)n;
    }
    return sum / 3;    /* divide by 3 to get v8(n!) */
}
