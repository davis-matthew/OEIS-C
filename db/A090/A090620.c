/*
 * A090620  Exponent of highest power of 13 dividing n!.
 * https://oeis.org/A090620
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n/13) + floor(n/169) + floor(n/2197) + ...
 *            (Legendre's formula for prime 13)
 * Range:     u8 n <= 3080, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;       /* Accumulate the exponent of 13 in n! */
    idx_t m = n;         /* Working copy of n */
    while (m) {
        m /= 13;                      /* Divide by 13 each step */
        sum += (seq_t)m;              /* Add floor(m/13) */
    }
    return sum;
}
