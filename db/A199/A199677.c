/*
 * A199677  a(n) = 8*9^n+1.
 * https://oeis.org/A199677
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*9^n + 1
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t base   = (seq_t)9;   /* base to raise */
    seq_t result = (seq_t)1;   /* accumulate 9^n */
    idx_t  e     = n;

    /* exponentiation by squaring: works modulo any unsigned width */
    while (e)
    {
        if (e & 1)
            result = result * base;
        base = base * base;
        e >>= 1;
    }

    result = result * (seq_t)8 + (seq_t)1;
    return result;
}
