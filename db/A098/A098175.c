/*
 * A098175  Smallest power of n with initial digit = 1 in decimal
 *          representation.
 * https://oeis.org/A098175
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 8, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Smallest power of n whose decimal representation starts with digit 1.
 * The algorithm multiplies successive powers of n until the most significant
 * digit becomes 1.  For arbitrary‑precision builds this works with the
 * standard integer arithmetic operators provided by the system.
 */
seq_t a(idx_t n)
{
    /* Special case n == 1 */
    if (n == 1)
        return (seq_t)1;

    seq_t base = (seq_t)n;   /* n itself */
    seq_t power = base;      /* current n^k, starting with k = 1 */
    const seq_t ten = (seq_t)10;

    for (;;)
    {
        /* Find most significant digit */
        seq_t tmp = power;
        while (tmp >= ten)
            tmp /= ten;

        if (tmp == (seq_t)1)
            return power;    /* found the required power */

        power *= base;       /* advance to next power */
    }
}
