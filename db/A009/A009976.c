/*
 * A009976  Powers of 32.
 * https://oeis.org/A009976
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 32^n Recurrence: a(0) = 1, a(n) = 32 * a(n-1) for n > 0
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Fast exponentiation by squaring */
    seq_t result = (seq_t)1;      /* 32^0 = 1 */
    seq_t base   = (seq_t)32;     /* the base 32 */
    idx_t e = n;

    while (e)
    {
        if (e & 1)
            result *= base;
        base *= base;
        e >>= 1;
    }
    return result;
}
