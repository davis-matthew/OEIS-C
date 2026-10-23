/*
 * A021763  Decimal expansion of 1/759.
 * https://oeis.org/A021763
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = decimal digit of 1/759 in position n+1 (0‑indexed)
 * Range:     u8 n <= 4, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t r = 1;   /* remainder before producing the first digit */
    for (idx_t i = 0; i <= n; ++i)
    {
        seq_t q = (r * 10) / 759;   /* digit at current position */
        if (i == n)
            return q;
        r = (r * 10) % 759;         /* next remainder */
    }
    /* Should never be reached */
    return 0;
}
