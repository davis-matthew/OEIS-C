/*
 * A024141  a(n) = 12^n - n.
 * https://oeis.org/A024141
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12^n - n
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 1;      /* 12^0 */
    seq_t base   = 12;     /* current power of 12 */
    idx_t   e     = n;

    while (e)
    {
        if (e & 1u) result *= base;   /* multiply when current bit is set */
        base *= base;                  /* square the base for next bit */
        e >>= 1;
    }

    result -= (seq_t)n;              /* subtract n */
    return result;
}
