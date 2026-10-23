/*
 * A197352  a(0)=0, a(1)=1, a(2n)=18*a(n), a(2n+1)=a(2n)+1.
 * https://oeis.org/A197352
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 255, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0, a(1)=1, a(2n)=18*a(n), a(2n+1)=a(2n)+1 */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Determine the position of the most significant bit. */
    idx_t temp = n;
    idx_t bits = 0;
    while (temp) {
        temp >>= 1;
        ++bits;
    }

    seq_t result = 0;
    /* Process bits from most significant to least significant. */
    for (idx_t i = bits; i-- > 0; ) {
        seq_t bit = (seq_t)((n >> i) & 1);
        result = result * 18 + bit;
    }

    return result;
}
